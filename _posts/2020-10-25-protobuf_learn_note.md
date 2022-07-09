---
layout:     post
title:      protobuf学习笔记
subtitle:   protobuf学习笔记
date:       2020-10-25
author:     敬方
header-img: img/post-bg-ios10.jpg
catalog: true
tags:
    - C++
    - 序列化
---

# protobuf 学习笔记

> 2020-10-08 19:57:58

## 0 参考链接
- [官方文档](https://developers.google.com/protocol-buffers/docs/reference/cpp-generated)
- [深入 ProtoBuf-编码](https://www.jianshu.com/p/73c9ed3a4877)
- [深入 ProtoBuf - 序列化源码解析](https://www.jianshu.com/p/62f0238beec8)
- [Protobuf 语法指南](https://colobu.com/2015/01/07/Protobuf-language-guide/)
- [protobuff源码](https://github.com/protocolbuffers/protobuf)
- [flatbuffers源码](https://github.com/google/flatbuffers)


## 1. 基础语法
下面是一个简单的使用示例:

```c++
message SearchRequest {
  required string query = 1;
  optional int32 page_number = 2;
  optional int32 result_per_page = 3; [default=10] //设置默认值
}
```

其中后面的数字编号表示；二进制格式中识别各个字段的，一旦开始使用就不能再改变，[1,15]之内的标识号在编码的时候会占用一个字节，之后的[16,2047]会占用两个字节

### 1.1 指定字段规则:
- required: 表示该字段必须
- optional: 可选项，表示该字段必须有0个或者1个值(不超过一个)
- repeated: 可重复值，相当于List
基本数值类型的repeated的字段并没有被尽可能地高效编码。在新的代码中，用户应该使用特殊选项[packed=true]来保证更高效的编码。如：

**注意:在proto3中没有这些选项**

```shell
repeated int32 samples = 4 [packed=true];
```


### 1.2 基本数据类型

proto中定义的基本数据类型会对应不同语言的基本数据类型如下:

|.proto Type|Notes	|C++ Type|Java Type|Python Type|Go Type|
|:---:|:---|:---:|:---:|:---:|:---:|
|double|	|double|double|float|*float64|
|float|		|float|float|float|*float32|
|int32|Uses variable-length encoding. Inefficient for encoding negative numbers – if your field is likely to have negative values, use sint32 instead.|int32|int|int|*int32|
|int64|Uses variable-length encoding. Inefficient for encoding negative numbers – if your field is likely to have negative values, use sint64 instead.|int64|long|int/long|*int64|
|uint32|Uses variable-length encoding.|uint32|int|int/long|*uint32|
|uint64|Uses variable-length encoding.|uint64|long|int/long|	*uint64|
|sint32|Uses variable-length encoding. Signed int value. These more efficiently encode negative numbers than regular int32s.|int32|int|int|*int32|
|sint64|Usesvariable-length encoding. Signed int value. These more efficiently encode negative numbers than regular int64s.	|int64|long|int/long|*int64|
|fixed32|Always four bytes. More efficient than uint32 if values are often greater than 228.|uint32|int|int/long|*uint32|
|fixed64|Always eight bytes. More efficient than uint64 if values are often greater than 256.|uint64|long|int/long|*uint64|
|sfixed32|Always four bytes.|int32|int|int|*int32|
|sfixed64|Always eight bytes.|int64|long|int/long|*int64|
|bool||bool|boolean|bool|*bool|
|string|A string must always contain UTF-8 encoded or 7-bit ASCII text.|string|String|unicode (Python 2) or str (Python 3)|*string|
|bytes|May contain any arbitrary sequence of bytes.	|string|	ByteString|	bytes|[]byte|

### 1.3 枚举

proto 中可以使用枚举类型作为关键的选择类；同时可以为枚举常量定义别名。需要设置allow_alias option 为 true, 否则 protocol编译器会产生错误信息。
```shell
enum EnumAllowingAlias {
  option allow_alias = true;
  UNKNOWN = 0;
  STARTED = 1;
  RUNNING = 1;
}

enum EnumNotAllowingAlias {
  UNKNOWN = 0;
  STARTED = 1;
  // RUNNING = 1;  // Uncommenting this line will cause a compile error inside Google and a warning message outside.
}
```

### 1.4 扩展

proto中允许使用扩展，通过定义相关的序列化字段，允许子类进行进一步的扩展；使用示例如下:

```shell
// 
message Foo {
  // ...
  // 定义可扩展字段
  extensions 100 to 199;
}

// 继承并定义

extend Foo {
  optional int32 bar = 126;
}
```

### 1.5 导入定义

可以通过导入（importing）其他.proto文件中的定义来使用它们。要导入其他.proto文件的定义，你需要在你的文件中添加一个导入声明，如：
```shell
import "myproject/other_protos.proto";
```

### 1.6 Oneof

如果你的消息中有很多可选字段， 并且同时至多一个字段会被设置， 你可以加强这个行为，使用oneof特性节省内存.

Oneof字段就像可选字段， 除了它们会共享内存， 至多一个字段会被设置。 设置其中一个字段会清除其它oneof字段。 你可以使用case()或者WhichOneof() 方法检查哪个oneof字段被设置，使用示例如下:
```shell
message SampleMessage {
  oneof test_oneof {
     string name = 4;
     SubMessage sub_message = 9;
  }
}
```
然后你可以增加oneof字段到 oneof 定义中. 你可以增加任意类型的字段, 但是不能使用 required, optional, repeated 关键字.

在产生的代码中, oneof字段拥有同样的 getters 和setters， 就像正常的可选字段一样. 也有一个特殊的方法来检查到底那个字段被设置. 你可以在相应的语言API中找到oneof API介绍.

Oneof 特性:
- 设置oneof会自动清楚其它oneof字段的值. 所以设置多次后，只有最后一次设置的字段有值.
- oneof不支持扩展.
- oneof不能 repeated.
- 反射API对oneof 字段有效.
- 如果使用C++,需确保代码不会导致内存泄漏. 下面的代码会崩溃， 因为sub_message 已经通过set_name()删除了.

```c++
SampleMessage message;
SubMessage* sub_message = message.mutable_sub_message();
message.set_name(“name”);      // Will delete sub_message
sub_message.set_…         
```

### 1.7 包(package)

当然可以为.proto文件新增一个可选的package声明符，用来防止不同的消息类型有命名冲突

```shell
package foo.bar;
message Open { ... }
```

### 1.8 定义服务(Service)

可以在.proto文件中定义一个RPC服务接口，protocol buffer编译器将会根据所选择的不同语言生成服务接口代码及存根。如，想要定义一个RPC服务并具有一个方法，该方法能够接收 SearchRequest并返回一个SearchResponse，此时可以在.proto文件中进行如下定义：
```shell
service SearchService {
  rpc Search (SearchRequest) returns (SearchResponse);
}
```
protocol编译器将产生一个抽象接口SearchService以及一个相应的存根实现。存根将所有的调用指向RpcChannel，它是一 个抽象接口，必须在RPC系统中对该接口进行实现。如，可以实现RpcChannel以完成序列化消息并通过HTTP方式来发送到一个服务器。换句话说， 产生的存根提供了一个类型安全的接口用来完成基于protocolbuffer的RPC调用，而不是将你限定在一个特定的RPC的实现中。C++中的代码 如下所示：
```c++
using google::protobuf;
protobuf::RpcChannel* channel;
protobuf::RpcController* controller;
SearchService* service;
SearchRequest request;
SearchResponse response;
void DoSearch() {
  // You provide classes MyRpcChannel and MyRpcController, which implement
  // the abstract interfaces protobuf::RpcChannel and protobuf::RpcController.
  channel = new MyRpcChannel("somehost.example.com:1234");
  controller = new MyRpcController;
  // The protocol compiler generates the SearchService class based on the
  // definition given above.
  service = new SearchService::Stub(channel);
  // Set up the request.
  request.set_query("protocol buffers");
  // Execute the RPC.
  service->Search(controller, request, response, protobuf::NewCallback(&Done));
}
void Done() {
  delete service;
  delete channel;
  delete controller;
}
```
所有service类都必须实现Service接口，它提供了一种用来调用具体方法的方式，即在编译期不需要知道方法名及它的输入、输出类型。在服务器端，通过服务注册它可以被用来实现一个RPC Server。

```c++
using google::protobuf;
class ExampleSearchService : public SearchService {
 public:
  void Search(protobuf::RpcController* controller,
              const SearchRequest* request,
              SearchResponse* response,
              protobuf::Closure* done) {
    if (request->query() == "google") {
      response->add_result()->set_url("http://www.google.com");
    } else if (request->query() == "protocol buffers") {
      response->add_result()->set_url("http://protobuf.googlecode.com");
    }
    done->Run();
  }
};
int main() {
  // You provide class MyRpcServer.  It does not have to implement any
  // particular interface; this is just an example.
  MyRpcServer server;
  protobuf::Service* service = new ExampleSearchService;
  server.ExportOnPort(1234, service);
  server.Run();
  delete service;
  return 0;
}
```

### 1.9 选项（Options）

在定义.proto文件时能够标注一系列的options。Options并不改变整个文件声明的含义，但却能够影响特定环境下处理方式。完整的可用选项可以在google/protobuf/descriptor.proto找到。

一些选项是文件级别的，意味着它可以作用于最外范围，不包含在任何消息内部、enum或服务定义中。一些选项是消息级别的，意味着它可以用在消息定 义的内部。当然有些选项可以作用在域、enum类型、enum值、服务类型及服务方法中。到目前为止，并没有一种有效的选项能作用于所有的类型。

常用选项类型如下:
- optimize_for (fileoption): 可以被设置为 SPEED, CODE_SIZE,or LITE_RUNTIME。这些值将通过如下的方式影响C++及java代码的生成：
  - SPEED (default): protocol buffer编译器将通过在消息类型上执行序列化、语法分析及其他通用的操作。这种代码是最优的。
  - CODE_SIZE: protocol buffer编译器将会产生最少量的类，通过共享或基于反射的代码来实现序列化、语法分析及各种其它操作。采用该方式产生的代码将比SPEED要少得多， 但是操作要相对慢些。当然实现的类及其对外的API与SPEED模式都是一样的。这种方式经常用在一些包含大量的.proto文件而且并不盲目追求速度的 应用中。
  - LITE_RUNTIME: protocol buffer编译器依赖于运行时核心类库来生成代码（即采用libprotobuf-lite 替代libprotobuf）。这种核心类库由于忽略了一 些描述符及反射，要比全类库小得多。这种模式经常在移动手机平台应用多一些。编译器采用该模式产生的方法实现与SPEED模式不相上下，产生的类通过实现 MessageLite接口，但它仅仅是Messager接口的一个子集。
- cc_generic_services, java_generic_services, py_generic_services (file options):protocol buffer编译器是否应该基于服务定义产生抽象服务代码。
- message_set_wire_format (message option)：如果该值被设置为true，该消息将使用一种不同的二进制格式来与Google内部的MessageSet的老格式相兼容。对于Google外部的用户来说，该选项将不会被用到。
- packed (field option): 如果该选项在一个整型基本类型上被设置为真，则采用更紧凑的编码方式。

### 1.10 自定义选项

ProtocolBuffers允许自定义并使用选项。该功能应该属于一个高级特性，对于大部分人是用不到的。由于options是定在 google/protobuf/descriptor.proto中的，因此你可以在该文件中进行扩展，定义自己的选项。如：
```shell
import "google/protobuf/descriptor.proto";
extend google.protobuf.MessageOptions {
  optional string my_option = 51234;
}
message MyMessage {
  option (my_option) = "Hello world!";
}
```

在上述代码中，通过对MessageOptions进行扩展定义了一个新的消息级别的选项。当使用该选项时，选项的名称需要使用（）包裹起来，以表明它是一个扩展。在C++代码中可以看出my_option是以如下方式被读取的。

```c++
string value = MyMessage::descriptor()->options().GetExtension(my_option);
continue
```