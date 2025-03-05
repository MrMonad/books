struct TickMsg;
class OB;
class EMA;
class MAD;
class ForwardAlgo;
class MedianAlgo;
class SwitchAlgo;
class BGNAlgo;
class FDCFilter;

typedef bsl::pair<TickMsg, TickMsg> BidaskPair


// Visitor Pattern
class AlgoVisitor {
public:
    virtual void visit(const EMA& ema) = 0;
    virtual void visit(const MAD& mad) = 0;
    virtual void visit(const ForwardAlgo& fa) = 0;
    virtual void visit(const MedianAlgo& ma) = 0;
    virtual void visit(const SwitchAlgo& sa) = 0;
    virtual void visit(const BGNAlgo& bgn) = 0;
};

// Strategy Pattern
class Algo {
public:
    virtual void update(const OB& ob, const TickMsg& t) = 0;
    virtual BidaskPair get() = 0;
    virtual void accept(AlgoVisitor& visitor) const = 0;
    virtual ~Algo() = default;
};

class OB {
public:
    void update(const TickMsg& t);
    bsl::pair<bsl::ranges::begin, bsl::ranges::end> get();
};

class EMA {
    double d_ema = 0.0;
public:
    void update(double value);
    double get() const;
    void accept(AlgoVisitor& visitor) const { visitor.visit(*this); }
};

class MAD {
    bsl::vector<double> history;
    double d_value;
public:
    void update(double value);
    double get() const { return d_value; }
    void accept(AlgoVisitor& visitor) const { visitor.visit(*this); }
};


class ForwardAlgo : public Algo {
    EMA d_ema;
    MAD d_madAsk; 
    MAD d_madBid;
public:
    void update(const OB& ob, const TickMsg& t) override {
        auto mid = medianMid(ob.get());
        auto spread = medianSpread(ob.get());
        d_ema.update(t.price);
        bid =  mid + d_ema.get()/2
        d_madAsk.update(t.price);
        ask =  mid + d_ema.get()/2
        d_madBid.update(t.price);
    }
    BidaskPair get(const OB& ob) override {
        TickMsg bid, ask;
        bid.price = d_madBid.get();
        ask.price = d_madAsk.get();
        // continue
    }
    void accept(AlgoVisitor& visitor) const override {
        d_ema.accept(visitor);
        d_madAsk.accept(visitor);
        d_madBid.accept(visitor);
        visitor.visit(*this);
    }
};


class MedianAlgo : public Algo {
    MAD d_madPrice;
public:
    void update(const OB& ob, const TickMsg& t) override {
        d_madPrice.update(t.price);
    }
    BidaskPair get(const OB& ob) override {
        return d_madPrice.get();
    }
    void accept(AlgoVisitor& visitor) const override {
        visitor.visit(*this);
        d_madPrice.accept(visitor);
    }
};

// Composite Pattern
class SwitchAlgo : public Algo {
    ForwardAlgo m_fa;
    MedianAlgo m_ma;
    int m_algoselection = 0;
public:
    void update(const OB& ob, const TickMsg& t) override {
        m_fa.update(ob, t);
        m_ma.update(ob, t);
    }
    BidaskPair get(const OB& ob) const override {
        return (m_algoselection == 0) ? m_fa.get(ob) : m_ma.get(ob);
    }
    void setAlgoSelection(int selection) { m_algoselection = selection; }
    void accept(AlgoVisitor& visitor) const override {
        visitor.visit(*this);
        m_fa.accept(visitor);
        m_ma.accept(visitor);
    }
};

// BGN Algo
class BGNAlgo {
    SwitchAlgo m_algoSwitch;
    OB m_ob;
    int m_fdcSwitch;
    FDCFilter = m_fdc;
public:
    void update(const TickMsg& t) {
        m_ob.update(t);
        auto temp_ob = FDC(m_ob.get(), m_fdc);  // FDC filter
        m_algoSwitch.update(temp_ob, t);
    }
    double get(const TickMsg& bid, const TickMsg& ask) {
        return switchAlgo.get(ob);
    }
    void accept(AlgoVisitor& visitor) const {
        visitor.visit(*this);
        switchAlgo.accept(visitor);
    }
};

// Example visitor implementation
class AttributionVisitor : public AlgoVisitor {
    bsl::vector d_messages;
public:
    void visit(const EMA& ema) override {
        d_messages.push("EMA attribution method called.");
    }
    void visit(const MAD& mad) override {
        d_messages.push("MAD attribution method called.");
    }
    void visit(const ForwardAlgo& fa) override {
        d_messages.push("ForwardAlgo attribution method called.");
    }
    void visit(const MedianAlgo& ma) override {
        d_messages.push("MedianAlgo attribution method called.");
    }
    void visit(const SwitchAlgo& sa) override {
        d_messages.push("SwitchAlgo attribution method called.");
    }
    void visit(const BGNAlgo& bgn) override {
       d_messages.push("BGNAlgo attribution method called.");
    }
};

void recoveryMode() {
    bsl::vector<TickMsg> ticks;
    AttributionVisitor visitor;
    BGNAlgo bgnAlgo;

    for(auto t in ticks) {
        bgnAlgo.update(t1);
        bgn.accept(visitor)
    }
    
    return 0;
}

void liveMode() {
    bsl::vector<TickMsg> ticks;
    AttributionVisitor visitor;
    BGNAlgo bgnAlgo;

    for(auto t in ticks) {
        bgnAlgo.update(t1);
    }
    
    return 0;
}






















struct TickMsg {
    double price;
    int volume;
};

struct OB {
    std::vector<TickMsg> ticks;
};

struct EMA {
    double spread;
    int spreadCount;
};

struct MADFilter {
    double cachedResult;
};

struct ForwardState {
    EMA spreadEMA;
    MADFilter bidMAD;
    MADFilter askMAD;
    double cached_mid;
    double cached_bid;
    double cached_ask;
};

struct MedianState {
    MADFilter medianMAD;
};

struct M_algo {
    int algorithmSelection;
};

struct AllState {
    OB ob;
    ForwardState forwardState;
    MedianState medianState;
    M_algo algo;
};

// OB Updater
void UpdateOB(OB& ob, TickMsg t) {
    ob.ticks.push_back(t);
}

// OB Getter
std::vector<TickMsg> GetTicks(const OB& ob, std::function<bool(const TickMsg&)> filterLambda) {
    std::vector<TickMsg> filteredTicks;
    for (const auto& tick : ob.ticks) {
        if (filterLambda(tick)) {
            filteredTicks.push_back(tick);
        }
    }
    return filteredTicks;
}

// EMA Updater (Stateless Function)
void UpdateEMA(EMA& ema, int spread, int spreadCount) {
    ema.spread = spread;
    ema.spreadCount = spreadCount;
}

// EMA Getter
EMA GetEMA(const EMA& ema) {
    return ema;
}

// MAD Filter Updater
void UpdateMAD(MADFilter& mad, TickMsg t) {
    mad.cachedResult = t.price; // Placeholder logic for updating MAD
}

// MAD Filter Getter
double GetMAD(const MADFilter& mad) {
    return mad.cachedResult;
}

// Forward Updater
void UpdateForward(ForwardState& fs, const OB& ob, TickMsg t) {
    UpdateEMA(fs.spreadEMA, t.price, t.volume);
    UpdateMAD(fs.bidMAD, t);
    UpdateMAD(fs.askMAD, t);
    fs.cached_mid = (t.price + fs.cached_mid) / 2;
    fs.cached_bid = t.price;
    fs.cached_ask = t.price;
}

// Forward Getter
ForwardState GetForward(const ForwardState& fs, TickMsg bid, TickMsg ask, bool useCache = true) {
    ForwardState result = fs;
    if (!useCache) {
        result.cached_bid = bid.price;
        result.cached_ask = ask.price;
        result.cached_mid = (bid.price + ask.price) / 2;
    }
    return result;
}

// Median Updater
void UpdateMedian(MedianState& ms, const OB& ob, TickMsg t) {
    UpdateMAD(ms.medianMAD, t);
}

// Median Getter
MedianState GetMedian(const MedianState& ms, const OB& ob, TickMsg bid, TickMsg ask) {
    return ms;
}

// LiveUpdateAlgo Updater
void UpdateAlgoSwitch(AllState& as, TickMsg t) {
    UpdateOB(as.ob, t);
    UpdateForward(as.forwardState, as.ob, t);
    UpdateMedian(as.medianState, as.ob, t);
}

void UpdateAlgoSwitch(AllState& as, int alg_selection) {
    as.algo.algorithmSelection = alg_selection;
}

// LiveUpdateAlgo Getter
AllState GetAlgoSwitch(const AllState& as, TickMsg bid, TickMsg ask) {
    AllState result = as;
    result.forwardState = GetForward(as.forwardState, bid, ask);
    return result;
}

// Attribution Functions
void GetAttribution(const OB& ob) {
    std::cout << "OB contains " << ob.ticks.size() << " ticks." << std::endl;
}

void GetAttribution(const EMA& ema) {
    std::cout << "EMA Spread: " << ema.spread << ", Spread Count: " << ema.spreadCount << std::endl;
}

void GetAttribution(const MADFilter& mad) {
    std::cout << "MAD Cached Result: " << mad.cachedResult << std::endl;
}

void GetAttribution(const ForwardState& fs) {
    std::cout << "Forward State:" << std::endl;
    GetAttribution(fs.spreadEMA);
    GetAttribution(fs.bidMAD);
    GetAttribution(fs.askMAD);
    std::cout << "Cached Mid: " << fs.cached_mid << ", Cached Bid: " << fs.cached_bid << ", Cached Ask: " << fs.cached_ask << std::endl;
}

void GetAttribution(const MedianState& ms) {
    std::cout << "Median State:" << std::endl;
    GetAttribution(ms.medianMAD);
}

void GetAttribution(const M_algo& algo) {
    std::cout << "Algorithm Selection: " << algo.algorithmSelection << std::endl;
}

void GetAttribution(const AllState& as) {
    std::cout << "AllState Attribution:" << std::endl;
    GetAttribution(as.ob);
    GetAttribution(as.forwardState);
    GetAttribution(as.medianState);
    GetAttribution(as.algo);
}

// Main Function
int main() {
    AllState allState;
    TickMsg tick1 = {100.5, 10};
    TickMsg tick2 = {101.0, 15};
    TickMsg bid = {99.5, 20};
    TickMsg ask = {102.0, 25};

    UpdateAlgoSwitch(allState, tick1);
    UpdateAlgoSwitch(allState, tick2);
    UpdateAlgoSwitch(allState, 1);

    GetAttribution(allState);
    
    return 0;
}
