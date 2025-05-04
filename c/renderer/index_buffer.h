//
// Created by brendan on 5/3/25.
//

#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

typedef unsigned int index_buffer_id_t;

index_buffer_id_t ib_create(unsigned int const* indices, unsigned int count);
void ib_destroy(index_buffer_id_t ib);
void ib_bind(index_buffer_id_t ib);
void ib_unbind(index_buffer_id_t ib);

#endif // INDEX_BUFFER_H
