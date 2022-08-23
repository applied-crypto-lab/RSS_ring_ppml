#ifndef Q_STRUCT_H_
#define Q_STRUCT_H_

struct quant_param {
     float scale;
     int zero_point;
     quant_param() {
          // scale = _scale;
          // zero_point = _zero_point;
     }
     quant_param(float _scale, int _zero_point) {
          scale = _scale;
          zero_point = _zero_point;
     }
};

#endif