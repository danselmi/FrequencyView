#ifndef IMPORT_DATA_FEEDER
#define IMPORT_DATA_FEEDER

#include <libsigrokcxx/libsigrokcxx.hpp>

class wxEvtHandler;

class ImportDataFeeder
{
public:
    ImportDataFeeder(wxEvtHandler *handler, int id);
    ~ImportDataFeeder() = default;

    void data_feed_callback(std::shared_ptr<sigrok::Device> device, std::shared_ptr<sigrok::Packet> packet);

protected:
    wxEvtHandler *evtHandler_;
    int id_;
};

#endif


