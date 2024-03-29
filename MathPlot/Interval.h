#pragma once
#include <boost/numeric/interval.hpp>
#include <boost/numeric/interval/transc.hpp>

using Interval = boost::numeric::interval<double,
boost::numeric::interval_lib::policies<
	boost::numeric::interval_lib::save_state_nothing<boost::numeric::interval_lib::rounded_transc_std<double>>,
boost::numeric::interval_lib::checking_base<double>>>;