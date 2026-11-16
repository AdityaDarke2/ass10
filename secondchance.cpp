#include <bits/stdc++.h>
using namespace std;


struct Request {
    int page;
    char op;
};


struct Frame {
    int page = -1;
    int r = 0;
    int m = 0;
};


void print_sc_state(const vector<Frame>& frames, int ptr) {
    for (int i = 0; i < (int)frames.size(); i++) {
        if (i == ptr) cout << "*";
        if (frames[i].page == -1) {
            cout << "[-] ";
        } else {
            cout << "[" << frames[i].page << " R:" << frames[i].r << "] ";
        }
    }
    cout << "\n";
}


void print_esc_state(const vector<Frame>& frames, int ptr, int written_page) {
    for (int i = 0; i < (int)frames.size(); i++) {
        if (i == ptr) cout << "*";
        if (frames[i].page == -1) {
            cout << "[-] ";
        } else {
            cout << "[" << frames[i].page << " R:" << frames[i].r << " M:" << frames[i].m << "] ";
        }
    }
    if (written_page != -1) {
        cout << " (disk write: page " << written_page << ")";
    }
    cout << "\n";
}


pair<int, int> run_second_chance(int n, const vector<Request>& reqs) {
    vector<Frame> frames(n);
    int ptr = 0, hits = 0, faults = 0;


    cout << "--- Second Chance ---\n";
    for (int step = 0; step < (int)reqs.size(); step++) {
        int p = reqs[step].page;
        bool hit = false;


        for (auto& f : frames) {
            if (f.page == p) {
                f.r = 1;
                hits++;
                hit = true;
                break;
            }
        }
        if (hit) continue;


        faults++;
        while (true) {
            if (frames[ptr].page == -1 || frames[ptr].r == 0) {
                frames[ptr].page = p;
                frames[ptr].r = 1;
                ptr = (ptr + 1) % n;
                break;
            } else {
                frames[ptr].r = 0;
                ptr = (ptr + 1) % n;
            }
        }


        cout << "Req " << p << ":R -> ";
        print_sc_state(frames, ptr);
    }


    cout << "Total Page Hits: " << hits << "\n";
    cout << "Total Page Faults: " << faults << "\n\n";
    return {hits, faults};
}


tuple<int, int, int> run_enhanced_second_chance(int n, const vector<Request>& reqs) {
    vector<Frame> frames(n);
    int ptr = 0, hits = 0, faults = 0, writes = 0;


    cout << "--- Enhanced Second Chance ---\n";
    for (int step = 0; step < (int)reqs.size(); step++) {
        int p = reqs[step].page;
        char op = reqs[step].op;
        bool hit = false;


        for (auto& f : frames) {
            if (f.page == p) {
                f.r = 1;
                if (op == 'W') f.m = 1;
                hits++;
                hit = true;
                break;
            }
        }
        if (hit) continue;


        faults++;
        int victim = -1;


        if (frames[ptr].page == -1) {
            victim = ptr;
        } else {
            for (int pass = 1; pass <= 4; pass++) {
                int target_m = (pass == 1 || pass == 3) ? 0 : 1;
                bool clear_r = (pass == 2);


                for (int i = 0; i < n; i++) {
                    if (frames[ptr].r == 0 && frames[ptr].m == target_m) {
                        victim = ptr;
                        break;
                    }
                    if (clear_r) frames[ptr].r = 0;
                    ptr = (ptr + 1) % n;
                }
                if (victim != -1) break;
            }
        }


        int written_page = -1;
        if (frames[victim].page != -1 && frames[victim].m == 1) {
            writes++;
            written_page = frames[victim].page;
        }


        frames[victim].page = p;
        frames[victim].r = 1;
        frames[victim].m = (op == 'W') ? 1 : 0;
        ptr = (victim + 1) % n;


        cout << "Req " << p << ":" << op << " -> ";
        print_esc_state(frames, ptr, written_page);
    }


    cout << "Total Page Hits: " << hits << "\n";
    cout << "Total Page Faults: " << faults << "\n";
    cout << "Total Pages Written to Disk: " << writes << "\n\n";
    return {hits, faults, writes};
}


int main() {
    int n;
    if (!(cin >> n)) return 0;


    string line;
    getline(cin >> ws, line);


    stringstream ss(line);
    string token;
    vector<Request> reqs;
    	
    while (ss >> token) {
        size_t pos = token.find(':');
        if (pos != string::npos) {
            int p = stoi(token.substr(0, pos));
            char op = toupper(token[pos + 1]);
            reqs.push_back({p, op});
        }
    }


    auto [sc_hits, sc_faults] = run_second_chance(n, reqs);
    auto [esc_hits, esc_faults, esc_writes] = run_enhanced_second_chance(n, reqs);


    cout << "--- Comparative Analysis ---\n";
    cout << "Second Chance          -> Faults: " << sc_faults
         << ", Disk Reads: " << sc_faults
         << ", Disk Writes: 0, Total Disk I/O: " << sc_faults << "\n";
    cout << "Enhanced Second Chance -> Faults: " << esc_faults
         << ", Disk Reads: " << esc_faults
         << ", Disk Writes: " << esc_writes
         << ", Total Disk I/O: " << (esc_faults + esc_writes) << "\n";


    if (esc_faults < sc_faults) {
        cout << "Result: Enhanced Second Chance had " << (sc_faults - esc_faults) << " fewer page faults.\n";
    } else if (esc_faults > sc_faults) {
        cout << "Result: Second Chance had " << (esc_faults - sc_faults) << " fewer page faults, but does not account for dirty page writes.\n";
    } else {
        cout << "Result: Both algorithms resulted in the same number of page faults (" << sc_faults << ").\n";
    }


    return 0;
}
