// [[Rcpp::plugins("cpp11")]]

#include <Rcpp.h>

#include <cpp11/environment.hpp>
#include <cpp11/function.hpp>
#include <cpp11/r_string.hpp>

//#include <R_ext/GraphicsEngine.h>

#include <vector>
#include <string>

#include "HttpgdDev.h"

namespace httpgd
{

    // returns system path to {package}/inst/www/{filename}
    std::string get_wwwpath(const std::string &filename)
    {
        using namespace cpp11::literals;

        auto sys_file = cpp11::package("base")["system.file"];

        cpp11::r_string res(sys_file("www", filename,
                                          "package"_nm = "httpgd"));
        return res;
    }

    inline HttpgdDev *getDev(pDevDesc dd)
    {
        return static_cast<HttpgdDev *>(dd->deviceSpecific);
    }

    // --------------------------------------

} // namespace httpgd

[[cpp11::register]]
bool httpgd_(std::string host, int port, std::string bg, double width, double height,
             double pointsize, Rcpp::List aliases, bool cors, std::string token, bool webserver, bool silent)
{
    bool recording = true;
    bool use_token = token.length();
    int ibg = R_GE_str2col(bg.c_str());

    std::string wwwpath(httpgd::get_wwwpath(""));

    auto dev = new httpgd::HttpgdDev(
        {host,
         port,
         wwwpath,
         cors,
         use_token,
         token,
         recording,
         webserver,
         silent},
        {ibg,
         width,
         height,
         pointsize,
         aliases});

    httpgd::HttpgdDev::make_device("httpgd", dev);
    return dev->server_start();
}

inline httpgd::HttpgdDev *validate_httpgddev(int devnum)
{
    if (devnum < 1 || devnum > 64) // R_MaxDevices
    {
        cpp11::stop("invalid graphical device number");
    }

    pGEDevDesc gdd = GEgetDevice(devnum - 1);
    if (!gdd)
    {
        cpp11::stop("invalid device");
    }
    pDevDesc dd = gdd->dev;
    if (!dd)
    {
        cpp11::stop("invalid device");
    }
    auto dev = static_cast<httpgd::HttpgdDev *>(dd->deviceSpecific);
    if (!dev)
    {
        cpp11::stop("invalid device");
    }

    return dev;
}

[[cpp11::register]]
Rcpp::List httpgd_state_(int devnum)
{
    auto dev = validate_httpgddev(devnum);

    auto svr_config = dev->api_server_config();
    httpgd::HttpgdState state = dev->api_state();

    return Rcpp::List::create(
        Rcpp::Named("host") = svr_config->host,
        Rcpp::Named("port") = dev->server_port(),
        Rcpp::Named("token") = svr_config->token,
        Rcpp::Named("hsize") = state.hsize,
        Rcpp::Named("upid") = state.upid,
        Rcpp::Named("active") = state.active);
}

[[cpp11::register]]
std::string httpgd_random_token_(int len)
{
    if (len < 0)
    {
        Rcpp::stop("Length needs to be 0 or higher.");
    }
    return httpgd::HttpgdDev::random_token(len);
}

[[cpp11::register]]
std::string httpgd_svg_(int devnum, int page, double width, double height)
{
    auto dev = validate_httpgddev(devnum);

    std::ostringstream buf;
    dev->api_svg(buf, page, width, height);
    return buf.str();
}

[[cpp11::register]]
bool httpgd_remove_(int devnum, int page)
{
    auto dev = validate_httpgddev(devnum);
    return dev->api_remove(page);
}

[[cpp11::register]]
bool httpgd_clear_(int devnum)
{
    auto dev = validate_httpgddev(devnum);
    return dev->api_clear();
}