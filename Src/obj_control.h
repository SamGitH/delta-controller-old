#ifndef OBJ_CONTROL_H_
#define OBJ_CONTROL_H_

#define ARRAY_ACTIONS_SIZE 300 //800 - максимум

void reset_seq_steps();
void translate_step(int n);
void do_demo(int state);
void add_object(float x, float y);
void obj_action(int i);
void encoder_pulce();
void put_obj();

#endif /* OBJ_CONTROL_H_ */
