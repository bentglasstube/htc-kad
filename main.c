#include <stdio.h>
#include <stdint.h>
#include <malloc.h>

typedef struct list {
  uint64_t * array;
  size_t length;
  size_t capacity;
} list;

// no error checking for speed
void list_push(list *list, uint64_t value) {
  if (list->length == list->capacity) {
    // need to allocate more space
    list->capacity *= 2;
    list->array = realloc(list->array, list->capacity * sizeof(uint64_t));
  }

  list->array[list->length] = value;
  list->length++;
}

typedef struct dist {
  uint64_t value;
  uint64_t distance;
} dist;

void merge(dist *output, dist* left, int left_count, dist* right, int right_count) {
  int i, j, k;

  i = j = k = 0;

  while (i < left_count && j < right_count) output[k++] = (left[i].distance < right[j].distance) ? left[i++] : right[j++];
  while (i < left_count) output[k++] = left[i++];
  while (j < right_count) output[k++] = right[j++];
}

void sort(dist *list, int n) {
  int mid, i;
  dist *left, *right;

  if (n < 2) return;

  mid = n / 2;

  left = (dist*)malloc(mid * sizeof(dist));
  right = (dist*)malloc((n - mid) * sizeof(dist));

  for (i = 0; i < mid; ++i) left[i] = list[i];
  for (i = mid; i < n; ++i) right[i - mid] = list[i];

  sort(left, mid);
  sort(right, n - mid);
  merge(list, left, mid, right, n - mid);

  free(left);
  free(right);
}

void nearest(uint64_t query, list *list, size_t count, uint64_t *output) {
  size_t i = 0;
  dist mins[count];

  for (i = 0; i < count; ++i) {
    mins[i].value = list->array[i];
    mins[i].distance = mins[i].value ^ query;
  }

  sort(mins, count);

  for (i = count; i < list->length; ++i) {
    dist d = {
      .value    = list->array[i],
      .distance = list->array[i] ^ query
    };

    if (d.distance < mins[count - 1].distance) {
      size_t j;
      for (j = count - 1; j > 0 && mins[j - 1].distance > d.distance; j--) {
        mins[j] = mins[j - 1];
      }
      mins[j] = d;
    }
  }

  for (i = 0; i < count; ++i) output[i] = mins[i].value;
}

int main(int argc, char **argv) {
  size_t request_size = 0;
  list values;

  sscanf(argv[1], "--request_size=%u", &request_size);

  values.length = 0;
  values.capacity = 2;
  values.array = malloc(values.capacity * sizeof(uint64_t));

  for (;;) {
    char *line;
    size_t len = 0;
    ssize_t read = getline(&line, &len, stdin);
    uint64_t value;

    if (read == 1) {
      free(line);
      break;
    }

    sscanf(line, "%lu", &value);
    list_push(&values, value);
  }

  for (;;) {
    char *line;
    size_t len = 0;
    ssize_t read = getline(&line, &len, stdin);
    uint64_t query;
    uint64_t results[request_size];

    if (read < 2) {
      free(line);
      break;
    }

    sscanf(line, "%lu", &query);
    nearest(query, &values, request_size, results);

    for (len = 0; len < request_size; ++len) {
      printf("%lu\n", results[len]);
    }
  }

  free(values.array);
}
