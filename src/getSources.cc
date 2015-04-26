#include "getSources.h"

using namespace v8;
using namespace WebRTC;

void GetSources::Init(Handle<Object> exports) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  exports->Set(String::NewFromUtf8(isolate, "getSources"), FunctionTemplate::New(isolate, GetSources::GetDevices)->GetFunction());
}

void GetSources::GetDevices(const FunctionCallbackInfo<Value>& args) {
  Isolate *isolate = args.GetIsolate();
  HandleScope scope(isolate);

  std::vector<cricket::Device> audio_devs;
  std::vector<cricket::Device> video_devs;
  rtc::scoped_ptr<cricket::DeviceManagerInterface> manager(cricket::DeviceManagerFactory::Create());

  if (manager->Init()) {
    Local<Array> list = Array::New(isolate);
    uint32_t index = 0;

    if (manager->GetAudioInputDevices(&audio_devs)) {
      std::vector<cricket::Device>::iterator audio_it;

      for (audio_it = audio_devs.begin(); audio_it != audio_devs.end(); audio_it++) {
        const cricket::Device &dev = *audio_it;
        Local<Object> dev_obj = Object::New(isolate);

        dev_obj->Set(String::NewFromUtf8(isolate, "kind"), String::NewFromUtf8(isolate, "audio"));
        dev_obj->Set(String::NewFromUtf8(isolate, "label"), String::NewFromUtf8(isolate, dev.name.c_str()));
        dev_obj->Set(String::NewFromUtf8(isolate, "id"), String::NewFromUtf8(isolate, dev.id.c_str()));

        list->Set(index, dev_obj);
        index++;
      }
    }

    if (manager->GetVideoCaptureDevices(&video_devs)) {
      std::vector<cricket::Device>::iterator video_it = video_devs.begin();

      for (video_it = video_devs.begin(); video_it != video_devs.end(); video_it++) {
        const cricket::Device &dev = *video_it;
        Local<Object> dev_obj = Object::New(isolate);

        dev_obj->Set(String::NewFromUtf8(isolate, "kind"), String::NewFromUtf8(isolate, "video"));
        dev_obj->Set(String::NewFromUtf8(isolate, "label"), String::NewFromUtf8(isolate, dev.name.c_str()));
        dev_obj->Set(String::NewFromUtf8(isolate, "id"), String::NewFromUtf8(isolate, dev.id.c_str()));

        list->Set(index, dev_obj);
        index++;
      }
    }

    return args.GetReturnValue().Set(list);
  }

  Local<Value> empty = Null(isolate);
  args.GetReturnValue().Set(empty);
}