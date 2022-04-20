#include "unknown.h"

const I_ID IID_IUnknown = 0;
const I_ID IID_IClassFactory = 10;

const I_ID IID_IMix = 1;
const I_ID IID_IPrint = 2;
const CLS_ID CLSIDColor = 101;

class IMix: public I_Unknown
{
public:
    virtual void mix(int r, int g, int b) = 0;
};

class IPrint: public I_Unknown
{
public:
    virtual void print() = 0;
};

class ColorFactory : public I_Unknown
{
    private:
        int countReference = 0;
    public:
        H_RESULT CreateInstance(I_ID iid, void** ppv, int r, int g, int b);
        H_RESULT QueryInterface(I_ID iid, void** ppv);
        U_LONG AddRef();
        U_LONG Release();
        ColorFactory();
};

H_RESULT GetClassObject(CLS_ID clsid, I_ID iid, void** ppv);

class ColorWrapper
{
    private:
        ColorFactory* factory;
        IMix* iMix;
        IPrint* iPrint;

    public:
        void print();
        void mix(int r, int g, int b);
        ColorWrapper(int r, int g, int b);
        ~ColorWrapper();
};