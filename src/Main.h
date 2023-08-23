#pragma once

int main(int argc, char** argv);
void display_help();
int parse_args(int argc, char** argv);
bool check_if_enough_parameters_available(int argc, int current_index, int parameters_required);
int run_action();
