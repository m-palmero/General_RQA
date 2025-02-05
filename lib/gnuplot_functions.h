# ifndef _gnuplot_functions_h
# define _gnuplot_functions_h

void plot_gnuplot_time_series(char title[], double parameter, int iter);
void plot_gnuplot_RP_matrix(char filename[], int N);
void plot_gnuplot_RPs_no_title(char filename[], int N);
void plot_gnuplot_RPs_title(char filename[], int N, int tau, int emb_dim, double eps);
void plot_gnuplot_bulk_rps(char filename[], int N);
void plot_gnuplot_RQA_distributions(char plot_name[], char *name_rqa, double avg, double eps, int size);
void plot_gnuplot_refining_RQA_distributions(char plot_name[], char *name_rqa, double avg, double eps, int size);
void plot_gnuplot_RQA_distributions_average(char plot_name[], char *name_rqa, double avg, double percentage, int N);
void plot_gnuplot_phase_space_title(char png_name[], double parameter, int iter);

# endif