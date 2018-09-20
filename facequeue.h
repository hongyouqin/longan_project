#ifndef FACEQUEUE_H
#define FACEQUEUE_H
#include <list>
#include <iterator>

//固定大小的队列
template<class T>
class FaceQueue
{
public:
    explicit FaceQueue(int num);

    void Push(const T& elem);

    typename std::list<T>::const_iterator Begin() const;

    typename std::list<T>::const_iterator End() const;

    int Len() const;

    int Limit() const;

    T GetAt(int index);

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
    return q_.cbegin();
}

template<class T>
typename std::list<T>::const_iterator FaceQueue<T>::End() const {
    return q_.cend();
}

template<class T>
int FaceQueue<T>::Len() const {
    return q_.size();
}

template<class T>
int FaceQueue<T>::Limit() const {
    return limit_num_;
}

template<class T>
T FaceQueue<T>::GetAt(int index)
{
    T t;
    typename std::list<T>::const_iterator iter = q_.cbegin();
    std::advance(iter, index);
    if (iter == q_.end()) {
        return t;
    }

    return *iter;
}




#endif // FACEQUEUE_H
