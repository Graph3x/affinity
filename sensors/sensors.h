

class ISensor{
    public:
        virtual int readValue();    
};

class DummySensor : ISensor{
    public:
        int readValue();    
};