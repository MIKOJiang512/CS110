#include "ringbuffer.h"
#include <stddef.h>

ring_buffer* ring_buffer_new(){
    ring_buffer *new_ring;
    new_ring = malloc(sizeof(ring_buffer));
    /*allocate *int*/
    new_ring->content = malloc(RING_BUFFER_INIT_SIZE*sizeof(int));
    new_ring->capacity = RING_BUFFER_INIT_SIZE;
    /*set initial pos as the same*/
    new_ring->read_pos = 0;
    new_ring->write_pos = 0;
    /*return a pointer point to this ring buffer*/
    return new_ring;
}

void ring_buffer_destroy(ring_buffer** buffer){
    free((*buffer)->content);/*free malloc(sizeof(int))*/
    free((*buffer));/*free malloc(sizeof(ring_buffer))*/
    *buffer = NULL;
}

bool ring_buffer_is_empty(const ring_buffer* buffer){
    if (buffer == NULL) {
     return false;/*illegally*/
    }
    if (buffer->read_pos == buffer->write_pos){/*in case of non_empty, read and write pos mustn't the same*/
        return true;
    }
    else {
        return false;
    }
}

bool ring_buffer_read(ring_buffer* buffer, int* data){
    if (ring_buffer_is_empty(buffer) || buffer == NULL) {/*in case of empty*/
        return false;
    }
    /*in case of success*/
    *data = buffer->content[buffer->read_pos];
    if(buffer->read_pos == buffer->capacity - 1){
        buffer->read_pos = 0;/*back to the first*/
    }
    else {
        buffer->read_pos ++;/*move to the next */
    }
    return true;
}

bool ring_buffer_write(ring_buffer* buffer, const int data){
    /*temporary various*/
    size_t previous_length;
    size_t i;
    /*new one*/
    ring_buffer *new_ring;
    if(buffer == NULL){/*illegal input*/
        return false;
    }

    /*to check if it need to extent*/
    /*cautious!!!!! check carefully if the condition is correct*/
    if ((buffer->write_pos + 1 == buffer->read_pos) || ((buffer->write_pos + 1 == buffer->capacity) && (buffer->read_pos == 0))){
        /*record the length*/
        previous_length = buffer->capacity;
        /*reallocate*/
        /*a totally new ringbuffer*/
        /*ring_buffer *new_ring;*/
        new_ring = malloc(sizeof(ring_buffer));
        /*allocate *int*/
        if (buffer->capacity < 1024){/*in case of < 1024*/
            new_ring->content = malloc(RING_BUFFER_GROW_FACTOR1*sizeof(int)*buffer->capacity);
            new_ring->capacity = RING_BUFFER_GROW_FACTOR1 * buffer->capacity;
        }
        else {/* in case of >=1024*/
            new_ring->content = malloc(RING_BUFFER_GROW_FACTOR2*sizeof(int)*buffer->capacity);
            new_ring->capacity = RING_BUFFER_GROW_FACTOR2 * buffer->capacity;
        }
        /*set new ops*/
        new_ring->read_pos = 0;
        new_ring->write_pos = previous_length - 1;
        /*copy it*/
        for (i = 0; i < previous_length - 1; i++) {
            ring_buffer_read(buffer, &(new_ring->content[i]));
        }
        /*move to the new home*/
        free(buffer->content);
        *buffer = *new_ring;
        free(new_ring);

    }

    /*normally write*/
    buffer->content[buffer->write_pos] = data;
    if (buffer->write_pos == buffer->capacity - 1){/*reached the end*/
        buffer->write_pos = 0;
    }
    else {
        buffer->write_pos ++;
    }
    return true;/*return true*/
}

bool ring_buffer_read_multi(ring_buffer* buffer, size_t rdsize, int* data){
    size_t i;
    if(buffer == NULL || data == NULL){/*illegal input*/
        return false;
    }
    /*check if there are enough elements*/
    if ((buffer->write_pos >= buffer->read_pos) && (buffer->write_pos - buffer->read_pos < rdsize)){       
            return false;
    }
    if ((buffer->write_pos < buffer->read_pos) && (buffer->write_pos - buffer->read_pos + buffer->capacity < rdsize)){       
            return false;
    }
    /*normally read*/
    for (i = 0; i < rdsize; i++) {
        ring_buffer_read(buffer, &(data[i]));/*read the buffer to *data*/
    }
    return true;
}

bool ring_buffer_write_multi(ring_buffer* buffer, size_t wrtsize, const int* data){
    size_t i = 0;
    if(buffer == NULL || data ==NULL){/*illegal input*/
        return false;
    }
    for (i = 0; i < wrtsize; i++) {/*use writefunction*/
        ring_buffer_write(buffer, data[i]);
    }
    return true;
}

bool ring_buffer_map(ring_buffer* buffer, map_func func){
    size_t i;
    if(buffer == NULL){/*illegal input*/
        return false;
    }
    for (i = 0; i < buffer->capacity; i++) {/*change all the element*/
        buffer->content[i] = func(buffer->content[i]);
    }
    return true;
}