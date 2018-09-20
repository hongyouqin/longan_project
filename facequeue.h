#ifndef FACEQUEUE_H
#define FACEQUEUE_H
#include <list>

//固定大小的队列
template<class T>
class FaceQueue
{
public:
    explicit FaceQueue(int num);

    void Push(const T& elem);

    typename std::list<T>::const_iterator Begin() const;

    typename std::list<T>::const_iterator End() const;

private:
    std::list<T> q_;

    int limit_num_;
};

template<class T>
FaceQueue<T>::FaceQueue(int num) : limit_num_(num)
{

}

template<class T>
void FaceQueue<T>::Push(const T &elem)
{
    if (q_.size() >= limit_num_) {
        q_.pop_back();
    }
    q_.push_front(elem);
}

template<class T>
typename std::list<T>::const_iterator FaceQueue<T>::Begin() const {
    return q_.begin();
}

template<class T>
typename std::list<T>::const_iterator FaceQueue<T>::End() const {
    return q_.end();
}


#endif // FACEQUEUE_H
