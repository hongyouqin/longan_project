#ifndef DATA_CENTER_H_
#define DATA_CENTER_H_

#include <memory>
#include <grpcpp/grpcpp.h>
#include "data_center_proto/dc_rpc.grpc.pb.h"

class FaceFeature;

class DataCenter {
public:
    DataCenter(std::shared_ptr<grpc::Channel> channel);

    //存储注册人脸
    bool StorageEmployeeFace(const ::proto::UserParam& request, ::proto::StorageReply* response);

    //存储陌生人人脸
    bool StorageStrangerFace(const ::proto::UserParam& request, ::proto::StorageReply* response);

    //拉取人脸注册表的所有数据
    bool ExtractFaceRegTableDatas(std::vector<std::shared_ptr<FaceFeature>> &faces);

private:
    std::unique_ptr<proto::LonganDataCenter::Stub> stub_;
};


#endif
