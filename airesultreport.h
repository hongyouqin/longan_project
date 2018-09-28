#ifndef AIRESULTREPORT_H
#define AIRESULTREPORT_H

#include <QObject>
#include <map>
#include <vector>
#include <memory>

class AiResult;
class FaceFeature;
class FaceAi;

//人脸识别结果
//组装线程结果
class AiResultReport : public QObject
{
    Q_OBJECT
public:
    explicit AiResultReport(QObject *parent = nullptr);

    void PushStranger(const FaceFeature &feature, int package_serial);

signals:

public slots:
    void RecvAiResult(const AiResult& result);

    void RecvEmployeeResult(const AiResult& result);

private:
    std::map<int, std::vector<std::shared_ptr<AiResult>>> result_map_;

    std::shared_ptr<FaceAi> ai_ = nullptr;
};

#endif // AIRESULTREPORT_H
