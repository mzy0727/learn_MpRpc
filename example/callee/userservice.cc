#include <iostream>
#include <string>
#include "user.pb.h"

using namespace fixbug;
/*
UserService原来是一个本地服务，提供了两个进程内的本地方法，Login和GetFriendLists
*/

class UserService:public UserServiceRpc{    // 使用在rpc服务的发布端（rpc服务提供者）
public:
    bool Login(std::string name, std::string pwd){
        std::cout << "doing local service Login"<<std::endl;
        std::cout<<"name:"<<name<<"pwd:"<<pwd<<std::endl;
    }
    /*
    重写基类UserServiceRpc的虚函数 下面这些方法都是框架直接调用的
    1. caller ==> Login(LoginRequest) ==> moduo ==> callee
    2. callee ==> Login(LoginRequest) ==> 交到下面重写的这个Login方法上了
    */ 
    void Login(::google::protobuf::RpcController* controller,
                    const ::fixbug::LoginRequest* request,
                    ::fixbug::LoginResponse* response,
                    ::google::protobuf::Closure* done){
        // 框架给业务上报了请求参数LoginRequest，应用获取相应数据做本地业务
        std::string name = request->name();
        std::string pwd = request->pwd();

        // 做本地业务
        bool login_result = Login(name,pwd);

        // 把响应写入 包括错误码、错误信息、返回值
        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_sucess(login_result);

        // 执行回调操作  
        // 执行响应对象数据的序列化和网络发送（都是由框架完成的）
        done->Run();
    }

};

int main(){
    UserService us;
    us.Login("xxx","xxx");

    return 0;
}