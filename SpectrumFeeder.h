#ifndef SPECTRUM_FEEDER
#define SPECTRUM_FEEDER

#include <libsigrokcxx/libsigrokcxx.hpp>

class wxEvtHandler;

class SpectrumFeeder
{
public:
    SpectrumFeeder(wxEvtHandler *handler, int id);
    ~SpectrumFeeder() = default;

    void data_feed_callback(std::shared_ptr<sigrok::Device> device, std::shared_ptr<sigrok::Packet> packet);

protected:
    void clear_trace();

    unsigned int sweep_points;
    double *values;

    wxEvtHandler *evtHandler_;
    int id_;
};

#endif

