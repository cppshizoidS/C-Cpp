#include "Component.h"
#include <iostream>

class Color : public IMix, IPrint
{
    private:
        int countReference = 0;
        int r, g, b;
        bool check(int r, int g, int b);
    public:
        H_RESULT QueryInterface(I_ID iid, void** ppv);
        U_LONG AddRef();
        U_LONG Release();
        void mix(int r, int g, int b);
        void print();
        Color(int r, int g, int b);
};

bool Color::check(int r, int g, int b) 
{
    if (r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255) return true;
    else return false;
}

Color::Color(int r, int g, int b)
{
    if (check(r, g, b))
    {
        this->r = r;
        this->g = g;
        this->b = b;
    } else throw 0;
}
void Color::mix(int r, int g, int b) 
{
    if (check(r, g, b))
    {
        this->r = (this->r + r) / 2;
        this->g = (this->g + g) / 2;
        this->g = (this->g + g) / 2;
    } else throw 0;

}

void Color::print() 
{
    std::cout << "Color: " << r << " " << g << " " << b << std::endl;
}

U_LONG Color::AddRef() 
{
    countReference++;
    return countReference;
}

U_LONG Color::Release() 
{
    countReference--;
    if(countReference == 0)
    {
        delete this;
    }
    return countReference;
}

H_RESULT Color::QueryInterface(I_ID iid, void** ppv)
{
    switch (iid)
    {
        case IID_IUnknown:
        {
            *ppv = (I_Unknown*) (IMix*) this;
            break;
        }
        case IID_IMix:
        {
            *ppv = (IMix*) this;
            break;
        }
        case IID_IPrint:
        {
            *ppv = (IPrint*) this;
            break;
        }
        default:
        {
            *ppv = 0;
            return E_NOINTERFACE;
        }
    }
    AddRef();
    return S_OK;
}

ColorFactory::ColorFactory()
{

}

U_LONG ColorFactory::AddRef() 
{
    countReference++;
    return countReference;
}

U_LONG ColorFactory::Release() 
{
    countReference--;
    if(countReference == 0)
    {
        delete this;
    }
    return countReference;
}
H_RESULT ColorFactory::CreateInstance(I_ID iid, void** ppv, int r, int g, int b)
{
    Color* color = new Color(r, g, b);
    if (color == 0)
    {
        return E_OUTOFMEMORY;
    }
    H_RESULT res = color->QueryInterface(iid, ppv);
    return res;
}

H_RESULT ColorFactory::QueryInterface(I_ID iid, void** ppv)
{
    if (iid == IID_IUnknown || iid == IID_IClassFactory)
    {
        *ppv = (I_ClassFactory*) this;
    }
    else
    {
        *ppv = 0;
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}

H_RESULT GetClassObject(CLS_ID clsid, I_ID iid, void** ppv)
{
    if(clsid != CLSIDColor)
    {
        return E_CLASSNOTAVAILABLE;
    }
    ColorFactory* factory = new ColorFactory();
    if(factory == 0) 
    {
        return E_OUTOFMEMORY;
    }
    H_RESULT res = factory->QueryInterface(iid, ppv);
    return res;
}

ColorWrapper::ColorWrapper(int r, int g, int b)
{
    GetClassObject(CLSIDColor, IID_IClassFactory, (void**)&factory);
    factory->CreateInstance(IID_IMix, (void**)&iMix, r, g, b);
    factory->Release();
    iMix->QueryInterface(IID_IPrint, (void**)&iPrint);
}

ColorWrapper::~ColorWrapper() 
{
    iMix->Release();
    iPrint->Release();
}

void ColorWrapper::print() 
{
    iPrint->print();
}

void ColorWrapper::mix(int r, int g, int b)
{
    iMix->mix(r, g, b);
}