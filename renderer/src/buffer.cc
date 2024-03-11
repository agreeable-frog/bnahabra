#include "buffer.hh"

template <class T>
void Buffer<T>::init() {
    glGenBuffers(1, &_id);
}

template <class T>
Buffer<T>::Buffer(Buffer::Target target, Buffer::Usage usage) : std::vector<T>(), _target(target), _usage(usage) {
    init();
}

template <class T>
Buffer<T>::Buffer(const Buffer<T>& other) : std::vector(std::dynamic_cast<std::vector<T>>(other)), _target(other.getTarget()), _usage(other.getUsage()) {
    init();
}

template <class T>
Buffer<T>& Buffer<T>::operator=(const Buffer<T>& other) {
    glDeleteBuffers(1, &_id);
    std::vector<T>::operator =(other);
    _target = other.getTarget();
    _usage = other.getUsage();
    _pipelineUid = 0;
    init();
}

template <class T>
Buffer<T>::~Buffer() {
    glDeleteBuffers(1, _&id);
}