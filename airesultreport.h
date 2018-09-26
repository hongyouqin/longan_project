#ifndef AIRESULTREPORT_H
#define AIRESULTREPORT_H

#include <QObject>
#include <map>
#include <vector>
#include <memory>

class AiResult;

//人脸识别结果
//组装线程结果
class AiResultReport : public QObject
{
    Q_OBJECT
public:
    explicit AiResultReport(QObject *parent = nullptr);

signals:

public slots:
    void RecvAiResult(const AiResult& result);

    void RecvEmployeeResult(const AiResult& result);

private:
    std::map<int, std::vector<std::shared_ptr<AiResult>>> result_map_;
};

#endif // AIRESULTREPORT_H
