#include "data_center.h"
#include "logger.h"
#include "facefeature.h"

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

bool  DataCenter::ExtractFaceRegTableDatas(std::vector<std::shared_ptr<FaceFeature>> &faces) {
   grpc::ClientContext context;
   ::proto::Empty empty;
   auto client = stub_->ExtractFaceRegTableDatas(&context, empty);
   ::proto::Feature employee;
   while (client->Read(&employee)) {
       auto face = employee.face();
       std::shared_ptr<FaceFeature> feature = std::make_shared<FaceFeature>();
       feature->name = face.name();
       feature->user_id_ = face.user_id();
       feature->face_photo = face.face_photo();
       feature->timestamp = face.acquisition_time();
       int size = (face.face_feature()).size();
       unsigned char* buf = (unsigned char*)((face.face_feature()).c_str());
       std::unique_ptr<unsigned char[], deleter> face_feature(new unsigned char[size](), auto_deleter);
       memcpy(face_feature.get(), buf, size);
       feature->feature_ = std::move(face_feature);
       feature->feature_size_ = face.face_size();
       faces.emplace_back(feature);
   }

   grpc::Status status = client->Finish();
   if (status.ok()) {
       return true;
   } else {
       LogE("ExtractFaceRegTableDatas call failed code = %d; error_msg = %s; ", status.error_code(), status.error_message().c_str());
   }

   return false;
}
