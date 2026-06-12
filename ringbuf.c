/*
 * ringbuf.c
 * Embedded C Ring Buffer Assignment
 *
 * Build:
 * gcc -Wall -std=c99 ringbuf.c -o ringbuf
 */

#include <stdio.h>
#include <stdint.h>

#define BUFFER_SIZE 8U

#define RINGBUF_OK     0
#define RINGBUF_ERROR -1

typedef struct
{
    uint8_t buffer[BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
} RingBuffer;

/* Initialize buffer */
void ringbuf_init(RingBuffer *rb)
{
    rb->head = 0U;
    rb->tail = 0U;
    rb->count = 0U;
}

/* Check if buffer is empty */
uint8_t ringbuf_is_empty(const RingBuffer *rb)
{
    return (rb->count == 0U);
}

/* Check if buffer is full */
uint8_t ringbuf_is_full(const RingBuffer *rb)
{
    return (rb->count == BUFFER_SIZE);
}

/* Get current number of stored bytes */
uint8_t ringbuf_count(const RingBuffer *rb)
{
    return rb->count;
}

/* Write one byte */
int ringbuf_write(RingBuffer *rb, uint8_t data)
{
    if (ringbuf_is_full(rb))
    {
        return RINGBUF_ERROR;
    }

    rb->buffer[rb->head] = data;

    /*
     * Faster than % BUFFER_SIZE on many MCUs because
     * it uses a bitwise AND instead of division.
     * Works only when BUFFER_SIZE is a power of 2.
     */
    rb->head = (rb->head + 1U) & (BUFFER_SIZE - 1U);

    rb->count++;

    return RINGBUF_OK;
}

/* Read one byte */
int ringbuf_read(RingBuffer *rb, uint8_t *data)
{
    if (ringbuf_is_empty(rb))
    {
        return RINGBUF_ERROR;
    }

    *data = rb->buffer[rb->tail];

    /*
     * Faster than % BUFFER_SIZE on many MCUs because
     * it uses a bitwise AND instead of division.
     * Works only when BUFFER_SIZE is a power of 2.
     */
    rb->tail = (rb->tail + 1U) & (BUFFER_SIZE - 1U);

    rb->count--;

    return RINGBUF_OK;
}

int main(void)
{
    RingBuffer rb;
    uint8_t data;
    uint8_t i;

    ringbuf_init(&rb);

    /* Step 1: Write 8 bytes */
    uint8_t first_data[8] =
    {
        0x41, 0x42, 0x43, 0x44,
        0x45, 0x46, 0x47, 0x48
    };

    for (i = 0U; i < 8U; i++)
    {
        if (ringbuf_write(&rb, first_data[i]) == RINGBUF_OK)
        {
            printf("[WRITE] 0x%02X -> OK (count=%u)",
                   first_data[i],
                   ringbuf_count(&rb));

            if (ringbuf_is_full(&rb))
            {
                printf(" FULL");
            }

            printf("\n");
        }
    }

    /* Step 2: Attempt extra write */
    if (ringbuf_write(&rb, 0x99U) == RINGBUF_ERROR)
    {
        printf("[WRITE] 0x99 -> FAIL (buffer full)\n");
    }

    /* Step 3: Read 3 bytes */
    for (i = 0U; i < 3U; i++)
    {
        if (ringbuf_read(&rb, &data) == RINGBUF_OK)
        {
            printf("[READ] -> 0x%02X (count=%u)\n",
                   data,
                   ringbuf_count(&rb));
        }
    }

    /* Step 4: Write 3 new bytes */
    uint8_t second_data[3] = {0x49, 0x4A, 0x4B};

    for (i = 0U; i < 3U; i++)
    {
        if (ringbuf_write(&rb, second_data[i]) == RINGBUF_OK)
        {
            printf("[WRITE] 0x%02X -> OK (count=%u)\n",
                   second_data[i],
                   ringbuf_count(&rb));
        }
    }

    /* Step 5: Read remaining 8 bytes */
    while (!ringbuf_is_empty(&rb))
    {
        if (ringbuf_read(&rb, &data) == RINGBUF_OK)
        {
            printf("[READ] -> 0x%02X (count=%u)\n",
                   data,
                   ringbuf_count(&rb));
        }
    }

    if (ringbuf_is_empty(&rb))
    {
        printf("Buffer is EMPTY\n");
    }

    /* Step 6: Attempt read from empty buffer */
    if (ringbuf_read(&rb, &data) == RINGBUF_ERROR)
    {
        printf("[READ] (empty) -> FAIL (buffer empty)\n");
    }

    return 0;
}
