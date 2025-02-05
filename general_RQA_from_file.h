// general_RQA_from_file.h
#ifndef GENERAL_RQA_FROM_FILE_H
#define GENERAL_RQA_FROM_FILE_H

int count_lines_in_file(const char *filename);
int give_recurrence_plot(int **R, int N);
void calculate_all_recurrence_quantifiers(int **R, int n);
void compute_recurrence_quantifiers_over_time(int **R, int n, int window_size, int step_size);
void recurrence_quantification_analysis(const char *input_file, double eps);


#endif // GENERAL_RQA_FROM_FILE_H
