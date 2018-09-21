#include "data_center.h"
#include "logger.h"

DataCenter::DataCenter(std::shared_ptr<grpc::Channel> channel)
    : stub_(proto::LonganDataCenter::NewStub(channel)) {

}

bool DataCenter::StorageEmployeeFace(const ::proto::UserParam& request, ::proto::StorageReply* response) {
    if (!stub_) {
        return false;
    }
    grpc::ClientContext context;
    grpc::Status status = stub_->StorageEmployeeFace(&context, request, response);
    if (status.ok()) {
        return true;
    } else {
        LogE("StorageEmployeeFace call failed code = %d; error_msg = %s; ", status.error_code(), status.error_message().c_str());
    }

    return false;
}

bool DataCenter::StorageStrangerFace(const ::proto::UserParam& request, ::proto::StorageReply* response) {
     if (!stub_) {
        return false;
    }
    grpc::ClientContext context;
    grpc::Status status = stub_->StorageStrangerFace(&context, request, response);
    if (status.ok()) {
        return true;
    } else {
        LogE("StorageStrangerFace call failed code = %d; error_msg = %s; ", status.error_code(), status.error_message().c_str());
    }

    return false;
}

bool DataCenter::ExtractFaceRegTableDatas(const proto::ExtractFaceParam &request, proto::StorageReply *response)
{
   if (!stub_) {
       return false;
   }
   grpc::ClientContext context;
   grpc::Status status = stub_->ExtractFaceRegTableDatas(&context, request, response);
   if (status.ok()) {
       return true;
   } else {
       LogE("ExtractFaceRegTableDatas call failed code = %d; error_msg = %s; ", status.error_code(), status.error_message().c_str());
   }
   return false;
}
