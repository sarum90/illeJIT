#pragma once

enum val_type {
  VAL_TYPE_UNDEFINED,
  VAL_TYPE_BOOL,
  VAL_TYPE_INT,
  VAL_TYPE_DOUBLE,
  VAL_TYPE_STRING,
};

struct val {
  enum val_type type;

  union {
    char b;
    int i;
    double d;
    char *s;
  };
};

static inline struct val mult_int(struct val *l, struct val *r) {
  struct val res;
  if(l->type != VAL_TYPE_INT ||
     r->type != VAL_TYPE_INT) {
    res.type = VAL_TYPE_UNDEFINED;
  } else {
    res.type = VAL_TYPE_INT;
    res.i = l->i * r->i;
  }
  return res;
}

static inline struct val add_int(struct val *l, struct val *r) {
  struct val res;
  if(l->type != VAL_TYPE_INT ||
     r->type != VAL_TYPE_INT) {
    res.type = VAL_TYPE_UNDEFINED;
  } else {
    res.type = VAL_TYPE_INT;
    res.i = l->i + r->i;
  }
  return res;
}

static inline struct val less_than_int(struct val *l, struct val *r) {
  struct val res;
  if(l->type != VAL_TYPE_INT ||
     r->type != VAL_TYPE_INT) {
    res.type = VAL_TYPE_BOOL;
    res.b = 0;
  } else {
    res.type = VAL_TYPE_BOOL;
    res.b = (l->i < r->i);
  }
  return res;
}

static inline struct val and_bool(struct val *l, struct val *r) {
  struct val res;
  if(l->type != VAL_TYPE_BOOL ||
     r->type != VAL_TYPE_BOOL) {
    res.type = VAL_TYPE_BOOL;
    res.b = 0;
  } else {
    res.type = VAL_TYPE_BOOL;
    res.b = (l->b && r->b);
  }
  return res;
}
