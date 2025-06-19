#pragma once

class ICommunicator {
   public:
        virtual int send_data() = 0;
        virtual bool has_data() = 0;
        virtual int read_data() = 0;
};