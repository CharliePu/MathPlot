#include "Rasterizer.h"

#include <array>
#include <queue>

Rasterizer::Rasterizer(): debugEnabled(false), thread(&Rasterizer::rasterizeTask, this)
{
}

Rasterizer::~Rasterizer()
{
    threadShouldClose = true;

    {
        std::lock_guard<std::mutex> requestReadyLock(requestReadyMutex), dataReadyLock(dataReadyMutex);
        requestReady = true;
        dataReady = false;
    }

    requestReadyCv.notify_one();
    thread.join();
}

void Rasterizer::rasterizeTask()
{
    while (!threadShouldClose)
    {
        std::unique_lock requestReadyLock(requestReadyMutex);
        requestReadyCv.wait(requestReadyLock, [this] {
            std::lock_guard<std::mutex> dataReadyLock(dataReadyMutex);
            return (requestReady && !dataReady);
        });

        requestReady = false;
        plot = requestPlot;
        width = requestWidth;
        height = requestHeight;
        requestReadyLock.unlock();

        if (threadShouldClose) {
            continue;
        }

        rasterize();

        requestReadyLock.lock();
        if (!requestReady)
        {
            std::lock_guard<std::mutex> dataReadyLock(dataReadyMutex);
            dataReady = true;
        }
        requestReadyLock.unlock();
    }
}

void Rasterizer::requestRasterize(Plot plot, int width, int height)
{
    if (plot.empty() || width <= 0 || height <= 0)
    {
        return;
    }

    std::lock_guard<std::mutex> requestReadyLock(requestReadyMutex);

    requestPlot = plot;
    requestWidth = width;
    requestHeight = height;
    requestReady = true;
    requestReadyCv.notify_one();
}

void Rasterizer::rasterize()
{
    auto getPixelInterval = [&](Interval interval, double width) {
	    const double lowerBound = interval.lower() / width;
	    const double upperBound = (interval.upper() + 1) / width;

        return Interval(lowerBound, upperBound);
    };

    auto mapToInterval = [&](Interval normalizedInterval, Interval mappedRange) {
	    const double scale = boost::numeric::width(mappedRange);
	    const double offset = mappedRange.lower();

        return Interval(normalizedInterval.lower() * scale + offset, normalizedInterval.upper() * scale + offset);
    };


    const auto expression = plot.getStatement().getExpression();
    const auto certaintyCheck = plot.getStatement().getIntervalCertaintyChecker();
    const auto comparator = plot.getStatement().getComparator();

    std::queue<IntervalNode*> nodeQueue;

    rootNode = std::make_unique<IntervalNode>();
    rootNode->xi = IInterval(0, width - 1);
    rootNode->yi = IInterval(0, height - 1);
    nodeQueue.push(rootNode.get());

    while (!nodeQueue.empty())
    {
	    const auto node = nodeQueue.front();
        nodeQueue.pop();

        Interval mappedXi = mapToInterval(getPixelInterval(node->xi, width), Interval(plot.getXMin(), plot.getXMax()));
        Interval mappedYi = mapToInterval(getPixelInterval(node->yi, height), Interval(plot.getYMin(), plot.getYMax()));

        auto evaluatedInterval = expression->evaluateInterval(mappedXi, mappedYi);

        //node->value = comparator(boost::numeric::median(evaluatedInterval), 0.0);


        // Blending lower bound and upper bound values
	    const double lbVal = comparator(evaluatedInterval.lower(), 0.0);
	    const double lbRat = abs(evaluatedInterval.lower()) / (abs(evaluatedInterval.lower()) + abs(evaluatedInterval.upper()));
	    const double ubVal = comparator(evaluatedInterval.upper(), 0.0);
	    const double ubRat = abs(evaluatedInterval.upper()) / (abs(evaluatedInterval.lower()) + abs(evaluatedInterval.upper()));
        node->value = lbVal * lbRat + ubVal * ubRat;

        if (certaintyCheck(evaluatedInterval)) {
            continue;
        }

        if (boost::numeric::width(node->xi) < 1.0 && boost::numeric::width(node->yi) < 1.0) {
            continue;
        }

        node->children = std::make_unique<std::array<IntervalNode, 2>>();

        if (boost::numeric::width(node->xi) >= 1.0 && 
            (boost::numeric::width(node->yi) <= boost::numeric::width(node->xi) || 
             boost::numeric::width(node->yi) < 1.0))
        {
            (*node->children)[0].xi = IInterval(node->xi.lower(), boost::numeric::median(node->xi));
            (*node->children)[0].yi = node->yi;

            (*node->children)[1].xi = IInterval(boost::numeric::median(node->xi) + 1, node->xi.upper());
            (*node->children)[1].yi = node->yi;
        }
        else if (boost::numeric::width(node->yi) >= 1.0)
        {
            (*node->children)[0].xi = node->xi;
            (*node->children)[0].yi = IInterval(node->yi.lower(), boost::numeric::median(node->yi));

            (*node->children)[1].xi = node->xi;
            (*node->children)[1].yi = IInterval(boost::numeric::median(node->yi) + 1, node->yi.upper());
        }

        nodeQueue.push(&(*node->children)[0]);
        nodeQueue.push(&(*node->children)[1]);
    }

    pixels.clear();
    pixels.reserve(height * width);
    for (int y = 0; y < height; y++)
    {
        //dMap.emplace_back();
        for (int x = 0; x < width; x++)
        {
            if (debugEnabled && checkDebugFrame(x, y))
            {
                pixels.push_back(255);
                pixels.push_back(255);
                pixels.push_back(255);
                pixels.push_back(255);
            }
            else
            {
                auto pixelBrightness = getPixel(x, y);
                pixels.push_back(pixelBrightness * 0);
                pixels.push_back(pixelBrightness * 41);
                pixels.push_back(pixelBrightness * 189);
                pixels.push_back(pixelBrightness * 153);
            }
        }

        // 4-byte alignment
        while (pixels.size() % 4 != 0)
        {
            pixels.push_back(0);
        }
    }
}

double Rasterizer::getPixel(int x, int y) const
{
    auto currNode = rootNode.get();

    while (currNode) {
        if (currNode->children) {
            for (auto &child : *currNode->children) {
                if (child.xi.lower() <= x && child.xi.upper() >= x &&
                    child.yi.lower() <= y && child.yi.upper() >= y) {
                    currNode = &child;
                    continue;
                }
            }
        }
        else {
            return currNode->value;
        }
    }
    
    return 0.0;
}

bool Rasterizer::checkDebugFrame(int x, int y) const
{
    auto currNode = rootNode.get();

    while (currNode) {
        if (currNode->children) {
            for (auto& child : *currNode->children) {
                if (child.xi.lower() <= x && child.xi.upper() >= x &&
                    child.yi.lower() <= y && child.yi.upper() >= y) {
                    currNode = &child;
                    continue;
                }
            }
        }
        else {
            return currNode->xi.lower() == x || currNode->xi.upper() == x ||
                currNode->yi.lower() == y || currNode->yi.upper() == y;
        }
    }

    return false;
}

void Rasterizer::toggleDebug()
{
    debugEnabled = !debugEnabled;
    requestRasterize(plot, width, height);
}

bool Rasterizer::isDataReady()
{
    std::lock_guard<std::mutex> dataReadyLock(dataReadyMutex);
    return dataReady;
}

std::vector<unsigned char> Rasterizer::getData()
{
    std::lock_guard<std::mutex> dataReadyLock(dataReadyMutex);
    dataReady = false;
    return pixels;
}

size_t Rasterizer::getWidth() const
{
    return width;
}

size_t Rasterizer::getHeight() const
{
    return height;
}
