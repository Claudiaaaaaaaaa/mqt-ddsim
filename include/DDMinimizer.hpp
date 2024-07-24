#include "Definitions.hpp"
#include "Permutation.hpp"
#include "QuantumComputation.hpp"

#include <cstddef>
#include <utility>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

namespace qc {
class DDMinimizer {


public:
  /**
   * @brief creates a vector hopefully the best initial Layout and applies it
   * @param QuantumComputation
   */
  static void optimizeInputPermutation(qc::QuantumComputation);


  static qc::Permutation createGateBasedPermutation(qc::QuantumComputation& qc);

  static bool isFull(std::vector<int> vec);
  static int getStairCount(std::vector<int> vec);
  static int getLadderPosition(std::vector<int> vec, int laadder);
  static std::vector<Qubit> reverseLayout(std::vector<Qubit> layout);
  static std::vector<Qubit> rotateRight(std::vector<Qubit> layout);
  static std::vector<Qubit> rotateLeft(std::vector<Qubit> layout);
  static std::pair<std::map<std::string, std::map<std::pair<Qubit, Qubit>, int>>,std::map<std::string, std::vector<int>>> makeDataStructure(qc::QuantumComputation& qc);


   /**
   * @brief creates a possible permutations of the initialLayout based on how to control bits are located
   * @param QuantumComputation
   */
  static qc::Permutation createControlBasedPermutation(qc::QuantumComputation& qc);

  /**
   * @brief creates a vector of all possible permutations of the initialLayout
   * @param QuantumComputation
   */
  static std::vector<qc::Permutation> createAllPermutations(qc::QuantumComputation& qc);

   /**
   * @brief Helper function to compute how many permutations there are for a number of qubits
   * @param number of Bits
   */
  static std::size_t factorial(std::size_t n);

}; // class DDMinimizer
} // namespace qc
