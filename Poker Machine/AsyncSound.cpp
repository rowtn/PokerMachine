#include "stdafx.h"
#include "AsyncSound.h"
#include <dshow.h>
#include <thread>

#pragma comment(lib, "strmiids.lib")

//From Matthew Walter's amazing googling skills
//https://gist.github.com/YoFuzzy3/9ecf1aa37f736e01fc7b

void playSound(const wchar_t* path) {
    IGraphBuilder *pGraph = 0;
    IMediaControl *pControl = 0;
    IMediaEvent *pEvent = 0;
    HRESULT hr = CoInitialize(0);
    if (FAILED(hr)) {
        return;
    }
    hr = CoCreateInstance(CLSID_FilterGraph, 0, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&pGraph);
    if (FAILED(hr)) {
        return;
    }
    hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
    hr = pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);
    hr = pGraph->RenderFile(path, 0);
    if (SUCCEEDED(hr)) {
        hr = pControl->Run();
        if (SUCCEEDED(hr)) {
            long evCode;
            pEvent->WaitForCompletion(INFINITE, &evCode);
        }
    }
    pEvent->Release();
    pControl->Release();
    pGraph->Release();
    CoUninitialize();
}

void playSoundAsync(const wchar_t* path) {
    std::thread t(playSound, path);
    t.detach();
}

