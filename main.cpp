#include "vector.h"
#include "matrix_util.h"
#include "prob_utils.h"
#include "hadamard_code.h"
#include <iostream>

int main() {
  hadamard_code hc(7);
  binary_matrix G = hc.generator_matrix();
  binary_matrix H = hc.parity_check_matrix();
  std::cout << "Hadamard-(" << G.rows() << "," << G.cols() << ")" << std::endl;

  /* this represents the system of equations to solve. use the parity check matrix */
  binary_matrix mat = H;

  /* pick some arbitrary "secret" solution vector to solve for. */
  /* careful not to exceed the error correction capabilities of the code */
  binary_vector error(mat.cols(), 0);
  error[3] = 1;
  error[4] = 1;
  error[5] = 1;
  error[7] = 1;
  error[30] = 1;

  /* syndrome vector becomes set of known variables */
  /* at the end we compare to error vector and check solution is correct */
  binary_vector syndrome = mat * error;

  /* initial variable values */
  real_vector iv_inputs(mat.cols(), prob_log(0.01)); /* prior over unknown variables */
  real_vector iv_outputs(mat.rows(), 0); /* known variables: the all-zero vector */

  for (size_t i = 0; i < iv_outputs.size(); ++i) {
    iv_outputs[i] = prob_log(syndrome[i].value());
  }

  /* lots of buffers needed */
  std::vector<std::vector<size_t>> vars(mat.cols());
  std::vector<std::vector<size_t>> var_indices(mat.cols());
  std::vector<std::vector<double>> var_beliefs0(mat.cols());
  std::vector<std::vector<double>> var_beliefs1(mat.cols());

  std::vector<std::vector<size_t>> checks(mat.rows());
  std::vector<std::vector<size_t>> check_indices(mat.rows());
  std::vector<std::vector<double>> check_beliefs0(mat.rows());
  std::vector<std::vector<double>> check_beliefs1(mat.rows());

  /* read matrix and set up graph edges and initial beliefs */
  for (size_t i = 0; i < mat.rows(); ++i) {
    for (size_t j = 0; j < mat.cols(); ++j) {

      var_indices[j].push_back(check_beliefs0[i].size());
      check_indices[i].push_back(var_beliefs0[j].size());

      if (mat[i][j].value()) {

        vars[j].push_back(i);
        checks[i].push_back(j);

        var_beliefs0[j].push_back(prob_not(iv_inputs[j]));
        var_beliefs1[j].push_back(iv_inputs[j]);

        check_beliefs0[i].push_back(prob_not(iv_outputs[i]));
        check_beliefs1[i].push_back(iv_outputs[i]);

      }
    }
  }

  /* perform bp iteration. exit early when solution is found */
  for (size_t n = 0; n < 100; ++n) {
    /* forward pass */
    for (size_t i = 0; i < vars.size(); ++i) {
      for (size_t j = 0; j < vars[i].size(); ++j) {
        double p0 = prob_not(iv_inputs[i]);
        double p1 = iv_inputs[i];
        for (size_t k = 0; k < vars[i].size(); ++k) {
          if (k != j) {
            double q0 = var_beliefs0[i][k];
            double q1 = var_beliefs1[i][k];
            p0 += q0;
            p1 += q1;
          }
        }
        double z = prob_sum(p0, p1);
        p0 -= z;
        p1 -= z;
        size_t check_parent = vars[i][j];
        size_t belief_index = var_indices[i][check_parent];
        check_beliefs0[check_parent][belief_index] = p0;
        check_beliefs1[check_parent][belief_index] = p1;
      }
    }

    /* backward pass */
    for (size_t i = 0; i < checks.size(); ++i) {
      for (size_t j = 0; j < checks[i].size(); ++j) {
        double p0 = prob_not(iv_outputs[i]);
        double p1 = iv_outputs[i];
        for (size_t k = 0; k < checks[i].size(); ++k) {
          if (k != j) {
            double q0 = check_beliefs0[i][k];
            double q1 = check_beliefs1[i][k];
            double r0 = prob_sum(p0 + q0, p1 + q1);
            double r1 = prob_sum(p1 + q0, p0 + q1);
            p0 = r0;
            p1 = r1;
          }
        }
        size_t var_parent = checks[i][j];
        size_t belief_index = check_indices[i][var_parent];
        var_beliefs0[var_parent][belief_index] = p0;
        var_beliefs1[var_parent][belief_index] = p1;
      }
    }

    /* iteration complete. perform a tentative hard-decision decoding */
    real_vector result(vars.size());
    real_vector result_hd(vars.size());
    for (size_t i = 0; i < vars.size(); ++i) {
      double p0 = prob_not(iv_inputs[i]);
      double p1 = iv_inputs[i];
      for (size_t j = 0; j < vars[i].size(); ++j) {
        double q0 = var_beliefs0[i][j];
        double q1 = var_beliefs1[i][j];
        p0 += q0;
        p1 += q1;
      }
      double z = prob_sum(p0, p1);
      result[i] = prob_exp(p1 - z);
      result_hd[i] = (result[i] < 0.5 ? 0 : 1);
    }

    /* matches expected result? */
    bool ok = true;
    for (size_t i = 0; i < result.size(); ++i) {
      if (result_hd[i] != error[i].value()) {
        ok = false;
        break;
      }
    }

    /* print the soft solution vector each iteration */
    std::cout << result << std::endl;
    if (ok) {
      std::cout << "Success! (" << n << " iterations)" << std::endl;
      std::cout << result_hd << std::endl;
      break;
    }
  }

  return 0;
}
