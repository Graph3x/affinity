
class ISensor{
    public:
        virtual int readValue();    
};

class DummyGrowingSensor : ISensor{
    public:
        int readValue();
    private:
        int value = 0; 
};