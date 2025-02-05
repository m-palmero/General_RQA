# ifndef _recurrence_functions_h
# define _recurrence_functions_h

typedef double (*distance_function)(double[], double[], int);

void embed_time_series(double **embedded_time_series, double *time_series, int n, int dim, int tau);
void manhattan_distance(double **embedded_time_series, int n, int dim, double **D);
void euclidean_distance(double **embedded_time_series, int n, int dim, double **D);
void supremum_distance(double **embedded_time_series, int n, int dim, double **D);
void modulated_distance(double **embedded_time_series, int n, int dim, double **D);
void calculate_distance_matrix(double **embedded_time_series, int n, int dim, double **D, const char *norm);
void compute_recurrence_matrix(double **D, int n, int **R, double threshold);
double compute_recurrence_rate(int **R, int n);
double adjust_threshold_via_recurrence_rate(double **D, int n, int **R, double desired_percentage, int *status);
int* calculate_diagonal_frequency_distribution(int** R, int n);
int* calculate_vertical_frequency_distribution(int** R, int n);
double compute_average_diagonal_length(int **R, int n);
int compute_longest_diagonal_length(int **R, int n);
double compute_average_vertical_length(int** R, int n);
int compute_longest_vertical_length(int **R, int n); 
double compute_recurrence_rate(int **R, int n);
double compute_determinism(int **R, int n);
double compute_laminarity(int **R, int n);
double compute_trapping_time(int **R, int n);
double compute_entropy_diagonal(int **R, int n);
double compute_entropy_vertical(int **R, int n);
double compute_divergence(int **R, int n);
double wrap_longest_diagonal_length(int **matrix, int size);
double wrap_longest_vertical_length(int **matrix, int size);



# endif