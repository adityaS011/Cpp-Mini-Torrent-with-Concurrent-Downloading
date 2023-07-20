#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <unistd.h> // For getopt

using namespace std;

class Peer {
public:
    Peer(const string& file_name, int num_peers);
    void startDownload();

private:
    void downloadFromPeer(int peer_id);
    void saveToFile(const string& file_name);

    string file_name_;
    int num_peers_;
    vector<int> peer_list_;
    vector<string> file_parts_;
    mutex mtx_;
};

Peer::Peer(const string& file_name, int num_peers)
    : file_name_(file_name), num_peers_(num_peers) {
    // Generate dummy file data (for demonstration purposes)
    ofstream file(file_name_);
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < 1000; ++i) {
        file << (rand() % 100) << " ";
    }
    file.close();

    // Split the file into parts based on the number of peers
    ifstream infile(file_name_);
    string content((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
    infile.close();

    int part_size = content.size() / num_peers;
    for (int i = 0; i < num_peers; ++i) {
        file_parts_.push_back(content.substr(i * part_size, part_size));
    }
}

void Peer::downloadFromPeer(int peer_id) {
    this_thread::sleep_for(chrono::milliseconds(1000 + rand() % 1000));

    // Simulate network latency and random data transmission delay
    lock_guard<mutex> lock(mtx_);
    cout << "Downloading part " << peer_id << " from peer " << peer_list_[peer_id] << endl;
    this_thread::sleep_for(chrono::milliseconds(2000 + rand() % 2000));
    file_parts_[peer_id] = "Downloaded part " + to_string(peer_id);
}

void Peer::saveToFile(const string& file_name) {
    ofstream outfile(file_name, ofstream::out | ofstream::trunc);
    for (const auto& part : file_parts_) {
        outfile << part;
    }
    outfile.close();
}

void Peer::startDownload() {
    // Generate peer list (for demonstration purposes)
    for (int i = 0; i < num_peers_; ++i) {
        peer_list_.push_back(i);
    }

    // Create threads for downloading from peers
    vector<thread> threads;
    for (int i = 0; i < num_peers_; ++i) {
        threads.emplace_back(&Peer::downloadFromPeer, this, i);
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }

    // Save the downloaded parts into a single file
    saveToFile("downloaded_file.txt");

    cout << "Download completed!" << endl;
}

int main(int argc, char* argv[]) {
    int num_peers = 1; // Default number of peers
    string file_name = "shared_file.txt"; // Default shared file name

    int option;
    const char* const short_opts = "hn:f:";
    const option long_opts[] = {
        {"help", no_argument, nullptr, 'h'},
        {"num-peers", required_argument, nullptr, 'n'},
        {"file", required_argument, nullptr, 'f'},
        {nullptr, 0, nullptr, 0}
    };

    while ((option = getopt_long(argc, argv, short_opts, long_opts, nullptr)) != -1) {
        switch (option) {
            case 'h':
                // Display help and usage information
                cout << "Usage: " << argv[0] << " [OPTIONS]\n"
                     << "Options:\n"
                     << "  -h, --help                Display this help message and exit.\n"
                     << "  -n, --num-peers <number>  Set the number of peers for concurrent downloading.\n"
                     << "  -f, --file <filename>     Specify the file to download (optional).\n";
                return 0;
            case 'n':
                num_peers = atoi(optarg);
                break;
            case 'f':
                file_name = optarg;
                break;
            case '?':
            default:
                cerr << "Unknown option or missing argument. Use --help for usage information.\n";
                return 1;
        }
    }

    Peer peer(file_name, num_peers);
    peer.startDownload();

    return 0;
}
