#ifndef DATA_FEEDER
#define DATA_FEEDER

#include <libsigrokcxx/libsigrokcxx.hpp>

class wxEvtHandler;

class DataFeeder
{
public:
    enum SourceType {
        NetworkAnalyzer,
        AnalogTimeSeries,
        DigitalTimeSeries
    };

    DataFeeder(wxEvtHandler *handler, SourceType sourceType);
    ~DataFeeder() = default;

    void data_feed_callback(std::shared_ptr<sigrok::Device> device, std::shared_ptr<sigrok::Packet> packet);

protected:
    void data_feed_callback_sa(std::shared_ptr<sigrok::Device> device, std::shared_ptr<sigrok::Packet> packet);
    void data_feed_callback_na(std::shared_ptr<sigrok::Device> device, std::shared_ptr<sigrok::Packet> packet);
    void data_feed_callback_ats(std::shared_ptr<sigrok::Device> device, std::shared_ptr<sigrok::Packet> packet);
    void data_feed_callback_dts(std::shared_ptr<sigrok::Device> device, std::shared_ptr<sigrok::Packet> packet);

    void clear_x_trace();
    void clear_y_trace();
    void clear_z_trace();

    const SourceType sourceType_;
    unsigned int sweep_points_x;
    unsigned int sweep_points_y;
    unsigned int sweep_points_z;
    double *x_vals;
    float *y_vals;
    double *z_vals;

    wxEvtHandler *evtHandler_;
};

#endif

