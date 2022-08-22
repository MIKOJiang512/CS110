template <typename _Tp>
RingBuffer<_Tp>::RingBuffer(){
    /*allocate *int*/
    _M_content = (_Tp*)malloc(RING_BUFFER_INIT_SIZE * sizeof(_Tp));
    _M_capacity = RING_BUFFER_INIT_SIZE;
    /*set initial pos as the same*/
    _M_read_pos = 0;
    _M_write_pos = 0;
    /*return a pointer point to this ring buffer*/
    /*return new_ring;*/
}

template <typename _Tp>
 RingBuffer<_Tp>::~RingBuffer(){
    delete _M_content;/*free malloc(sizeof(int))*/
    /*free malloc(sizeof(ring_buffer))*/
    /*self = NULL;*/
    _M_content = NULL;
    // all zero
    _M_read_pos = 0;
    _M_write_pos = 0;
    _M_capacity = 0;
    // all zero
 }

template <typename _Tp>
size_t RingBuffer<_Tp>::get_capacity() const{
    return _M_capacity;//get capacity
}

template <typename _Tp>
bool RingBuffer<_Tp>::is_empty() const{
    if (_M_read_pos == _M_write_pos){/*in case of non_empty, read and write pos mustn't the same*/
        return true;
    }
    else {
        //end
        return false;
    }
}


template <typename _Tp>
bool RingBuffer<_Tp>::read(_Tp& data){
    if (_M_read_pos == _M_write_pos) {/*in case of empty*/
        return false;
    }
    /*in case of success*/
    data = _M_content[_M_read_pos];
    if(_M_read_pos == _M_capacity - 1){
        _M_read_pos = 0;/*back to the first*/
    }
    else {
        _M_read_pos ++;/*move to the next */
    }
    return true;
}

template <typename _Tp>
bool RingBuffer<_Tp>::write(const _Tp& data){
    /*temporary various*/
    size_t previous_length;
    size_t i;
    /*new one*/
    _Tp* new_content;
    size_t new_M_capacity;
    //a new temp to copy
    //value_type temp[99999];

    /*to check if it need to extent*/
    /*cautious!!!!! check carefully if the condition is correct*/
    if ((_M_write_pos + 1 == _M_read_pos) || ((_M_write_pos + 1 == _M_capacity) && (_M_read_pos == 0))){
        /*record the length*/
        previous_length = _M_capacity;
        /*reallocate*/
        /*a totally new ringbuffer*/
        /*ring_buffer *new_ring;*/
        //new_ring = malloc(sizeof(RingBuffer));
        /*allocate *int*/
        if (_M_capacity < 1024){/*in case of < 1024*/
            new_content = (_Tp*)malloc(RING_BUFFER_GROW_FACTOR1*sizeof(_Tp)*_M_capacity);
            new_M_capacity = RING_BUFFER_GROW_FACTOR1 * _M_capacity;
        }
        else {/* in case of >=1024*/
            new_content = (_Tp*)malloc(RING_BUFFER_GROW_FACTOR2*sizeof(_Tp)*_M_capacity);
            new_M_capacity = RING_BUFFER_GROW_FACTOR2 * _M_capacity;
        }

        /*copy it to new_content*/
        for (i = 0; i < previous_length - 1; i++) {
            RingBuffer<_Tp>::read(new_content[i]);
        }
        //capacity change
        _M_capacity = new_M_capacity;

        /*set new ops*/
        _M_read_pos = 0;
        _M_write_pos = previous_length - 1;
        /*move to the new home*/
        free(_M_content);
        _M_content = new_content;
        //free(new_ring);
        new_content = NULL;
        
    }
    /*normally write*/
    _M_content[_M_write_pos] = data;
    if (_M_write_pos == _M_capacity - 1){/*reached the end*/
        _M_write_pos = 0;
    }
    else {
        _M_write_pos ++;
    }
    return true;/*return true*/
}

template <typename _Tp>
bool RingBuffer<_Tp>::read_multi(size_t rdsize, std::vector<_Tp>& data){
    size_t i;//temp i
    //resize the vector
    data.resize(rdsize);
    /*check if there are enough elements*/
    if ((_M_write_pos >= _M_read_pos) && (_M_write_pos - _M_read_pos < rdsize)){       
            return false;
    }
    if ((_M_write_pos < _M_read_pos) && (_M_write_pos - _M_read_pos + _M_capacity < rdsize)){       
            return false;
    }
    /*normally read*/
    for (i = 0; i < rdsize; i++) {
        RingBuffer<_Tp>::read(data[i]);/*read the buffer to *data*/
    }
    return true;
}

template <typename _Tp>
bool RingBuffer<_Tp>::write_multi(size_t wrtsize, const std::vector<_Tp>& data){
    size_t i = 0;// temp i
    if ((size_t)data.size() < wrtsize){
        /* end */
        return false;
    }
    for (i = 0; i < wrtsize; i++) {/*use writefunction*/
        this->write(data[i]);
    } 
    return true;
}

template <typename _Tp>
bool RingBuffer<_Tp>::map(std::function<_Tp(_Tp)>&& func){
    if (is_empty()) {/*in case of empty*/
        return false;
    }
    size_t i;// temp i
    for (i = 0; i < _M_capacity; i++) {/*change all the element*/
        _M_content[i] = func(_M_content[i]);
    }
    return true;
}

// iterator
template <typename _Tp>
class __detail::__iterator{
    private:
    //two pointer
        RingBuffer<_Tp> *m_buffer;// buffer is used to find capacity
        _Tp *m_ptr;
    public:
        //construct
        __iterator()
        {
            // nitialize a nullptr
            m_ptr = nullptr;
            m_buffer = nullptr;
        }
        //initialize with given factor
        __iterator( _Tp *ptr, RingBuffer<_Tp> *buffer){
            m_buffer = buffer;
            m_ptr = ptr;
        };
        //construct2
        __iterator(const __iterator<_Tp> &other)
        {
            // initialize iterator with the given iterator
            m_ptr = other.m_ptr;
            m_buffer = other.m_buffer;
        }
        //+
        __iterator operator+(ptrdiff_t x){
            __detail::__iterator<_Tp> temp = *this; //copy a temp iterator
            temp.m_ptr += x;
            while (temp.m_ptr >= temp.m_buffer->_M_content + temp.m_buffer->_M_capacity){//normalize it
                temp.m_ptr -= temp.m_buffer->_M_capacity; // also temp.m_buffer
            }
            return temp; // return the result
            
        };
        //-
        __iterator operator-(ptrdiff_t x){
            __detail::__iterator<_Tp> temp = *this; //copy a temp iterator
            temp.m_ptr -= x;
            while (temp.m_ptr < &temp.m_buffer->_M_content[0]){//normalize it
                temp.m_ptr += temp.m_buffer->_M_capacity; // also temp.m_buffer
            }
            return temp; // return the result
        };
        // =
        __iterator& operator=(__iterator & other){
            // copy the others
            m_ptr = other.m_ptr;
            m_buffer = other.m_buffer;
            return *this;
        };
        // +=
        __iterator& operator+=(ptrdiff_t x){
            m_ptr = m_ptr + x;
            while (m_ptr >= &m_buffer->_M_content[m_buffer->_M_capacity]){//normalize it
                m_ptr -= m_buffer->_M_capacity; //
            }
            return *this;
        };
        // -=
        __iterator& operator-=(ptrdiff_t x){
            m_ptr = m_ptr - x;
            while (m_ptr < &m_buffer->_M_content[0]){//normalize it
                m_ptr += m_buffer->_M_capacity; //
            }
            return *this;
        };
        // ++
        __iterator& operator++(){
            m_ptr = m_ptr + 1;// +1
            if (m_ptr >= &m_buffer->_M_content[m_buffer->_M_capacity]){//reach the end
                m_ptr = m_buffer->_M_content;
            }
            return *this;
        };
        // --
        __iterator& operator--(){
            m_ptr = m_ptr - 1;// -1
            if (m_ptr < &m_buffer->_M_content[0]){
                m_ptr =  &m_buffer->_M_content[m_buffer->_M_capacity - 1];//reach the head
            }
            return *this;
        };
        // ++int
        __iterator operator++(int){
            _Tp* temp = m_ptr; //copy a temp pointer
            m_ptr ++;
            if (m_ptr >= &m_buffer->_M_content[m_buffer->_M_capacity]){
                m_ptr = m_buffer->_M_content;// point to the beginning
            }
            return __detail::__iterator<_Tp>(temp, m_buffer);// return a new iterator
        };
        // --int
        __iterator operator--(int){
            _Tp* temp = m_ptr; //copy a temp pointer
            m_ptr --;
            if (m_ptr >= &m_buffer->_M_content[m_buffer->_M_capacity]){
                m_ptr = &m_buffer->_M_content[m_buffer->_M_capacity - 1];// point to the end
            }
            return __detail::__iterator<_Tp>(temp, m_buffer);// return a new iterator
        };
        //bool ==
        bool operator==(const __detail::__iterator<_Tp> & other){
            return (m_buffer == other.m_buffer && m_ptr == other.m_ptr);
        };
        //bool !=
        bool operator!=(const __detail::__iterator<_Tp> & other){
            return !(m_buffer == other.m_buffer && m_ptr == other.m_ptr);
        };
        // get value
        _Tp &operator*(){
            return *m_ptr;
        };
        // get pointer
        _Tp *operator->(){
            return m_ptr;
        };

};
// iterator begin()
template <typename _Tp>
__detail::__iterator<_Tp> RingBuffer<_Tp>::begin(){
    return __detail::__iterator<_Tp>(&(_M_content[_M_read_pos]), this);//(adrress of readpos, adrress of buffer)
}
// iterator end()
template <typename _Tp>
__detail::__iterator<_Tp> RingBuffer<_Tp>::end(){
    return __detail::__iterator<_Tp>(&(_M_content[_M_write_pos]), this);//(adrress of writepos, adrress of buffer)
}

// const iterator
template <typename _Tp>
class __detail::__const_iterator{
    private:
        const RingBuffer<_Tp> *m_buffer;// a const buffer
        _Tp *m_ptr;
    public:
        // Default constructor
        __const_iterator();
        // Copy constructor
        __const_iterator(const __iterator<_Tp> &other)
        {
            // initialize iterator with the given iterator
            m_ptr = other.m_ptr;
            m_buffer = other.m_buffer;
        }
        __const_iterator( _Tp *ptr, const RingBuffer<_Tp> *buffer){// const buffer
            m_buffer = buffer;
            m_ptr = ptr;
        };
        //+
        __const_iterator operator+(ptrdiff_t x){
            __detail::__const_iterator<_Tp> temp = *this; //copy a temp iterator
            temp.m_ptr += x;
            while (temp.m_ptr >= &temp.m_buffer->_M_content[temp.m_buffer->_M_capacity]){//normalize it
                temp.m_ptr -= temp.m_buffer->_M_capacity; // also temp.m_buffer
            }
            return temp; // return the result
            
        };
        //-
        __const_iterator operator-(ptrdiff_t x){
            __detail::__const_iterator<_Tp> temp = *this; //copy a temp iterator
            temp.m_ptr -= x;
            while (temp.m_ptr < &temp.m_buffer->_M_content[0]){//normalize it
                temp.m_ptr += temp.m_buffer->_M_capacity; // also temp.m_buffer
            }
            return temp; // return the result
        };
        // =
        __const_iterator& operator=(const __iterator<_Tp> & other){
            // copy the others
            m_ptr = other.m_ptr;
            m_buffer = other.m_buffer;
            return *this;
        };
        // +=
        __const_iterator& operator+=(ptrdiff_t x){
            m_ptr = m_ptr + x;
            while (m_ptr >= &m_buffer->_M_content[m_buffer->_M_capacity]){//normalize it
                m_ptr -= m_buffer->_M_capacity; //
            }
            return *this;
        };
        // -=
        __const_iterator& operator-=(ptrdiff_t x){
            m_ptr = m_ptr - x;
            while (m_ptr < &m_buffer->_M_content[0]){//normalize it
                m_ptr += m_buffer->_M_capacity; //
            }
            return *this;
        };
        // ++
        __const_iterator& operator++(){
            m_ptr = m_ptr + 1;// +1
            if (m_ptr >= &m_buffer->_M_content[m_buffer->_M_capacity]){//reach the end
                m_ptr = m_buffer->_M_content;
            }
            return *this;
        };
        // --
        __const_iterator& operator--(){
            m_ptr = m_ptr - 1;// -1
            if (m_ptr < &m_buffer->_M_content[0]){
                m_ptr =  &m_buffer->_M_content[m_buffer->_M_capacity - 1];//reach the head
            }
            return *this;
        };
        // ++int
        __const_iterator operator++(int){
            _Tp* temp = m_ptr; //copy a temp pointer
            m_ptr ++;
            if (m_ptr >= &m_buffer->_M_content[m_buffer->_M_capacity]){
                m_ptr = m_buffer->_M_content;// point to the beginning
            }
            return __detail::__const_iterator<_Tp>(temp, m_buffer);// return a new iterator
        };
        // --int
        __const_iterator operator--(int){
            _Tp* temp = m_ptr; //copy a temp pointer
            m_ptr --;
            if (m_ptr >= &m_buffer->_M_content[m_buffer->_M_capacity]){
                m_ptr = &m_buffer->_M_content[m_buffer->_M_capacity - 1];// point to the end
            }
            return __detail::__const_iterator<_Tp>(temp, m_buffer);// return a new iterator
        };
        //bool ==
        bool operator==(const __detail::__const_iterator<_Tp> & other){
            return (m_buffer == other.m_buffer && m_ptr == other.m_ptr);
        };
        //bool !=
        bool operator!=(const __detail::__const_iterator<_Tp> & other){
            return !(m_buffer == other.m_buffer && m_ptr == other.m_ptr);
        };
        // get value
        _Tp &operator*(){
            return *m_ptr;
        };
        // get pointer
        _Tp *operator->(){
            return m_ptr;// the pointer to my element
        };
     
};

template <typename _Tp>
__detail::__const_iterator<_Tp> RingBuffer<_Tp>::cbegin() const{//need a const???
    return __detail::__const_iterator<_Tp>(&(_M_content[_M_read_pos]), this);
    // this refer to Ringbiffer ????
}

template <typename _Tp>
__detail::__const_iterator<_Tp> RingBuffer<_Tp>::cend() const{//need a const???
    return __detail::__const_iterator<_Tp>(&(_M_content[_M_write_pos]), this);
}



    