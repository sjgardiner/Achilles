#include "nuchic/EventWriter.hh"
#include "nuchic/Event.hh"
#include "nuchic/Particle.hh"
#include "nuchic/Version.hh"
#include "fmt/format.h"

nuchic::NuchicWriter::NuchicWriter(const std::string &filename, bool zip) : toFile{true}, zipped{zip} {
#ifdef GZIP
    if(zipped) {
        std::string zipname = filename;
        if(filename.substr(filename.size() - 3) != ".gz")
            zipname += std::string(".gz");
        m_out = new ogzstream(zipname.c_str());
    } else
#endif
        m_out = new std::ofstream(filename);
}

void nuchic::NuchicWriter::WriteHeader(const std::string &filename) {
    *m_out << fmt::format("Nuchic Version: {}\n", NUCHIC_VERSION);
    *m_out << fmt::format("{0:-^40}\n\n", "");

    std::ifstream input(filename);
    std::string line;
    while(std::getline(input, line)) {
        *m_out << fmt::format("{}\n", line);
    }
    *m_out << fmt::format("{0:-^40}\n\n", "");
}

void nuchic::NuchicWriter::Write(const Event &event) {
    *m_out << fmt::format("Event: {}\n", ++nEvents);
    *m_out << fmt::format("  Particles:\n");
    for(const auto &part : event.Particles()) {
        *m_out << fmt::format("  - {}\n", part);
    }
    *m_out << fmt::format("  - {}\n", event.Remnant());
    *m_out << fmt::format("  Weight: {}\n", event.Weight());
}
