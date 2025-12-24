#ifndef _EMMA_SOLUTION_COMPOUNDS_H_
#define _EMMA_SOLUTION_COMPOUNDS_H_

#include "runtime/solutions/module/core.h"
#include "runtime/solutions/persona/core.h"
#include "utils/id.h"

/*

  Data:
  - Element: Core attributes, often native types (enum, bool, float...)
  - Compound: A group of elements
  - Solution: A set of Compounds, depending on formulas' intents, a solution
  might have a specific goal (increate engageement, drive retention...)

  Behavior:
    - Formula: Puts in relation and combine elements or compounds together to
  provide data required by the app such as Octalysis or Heatmap.



               Compound                                           . +
   ________________^_______________                             .  . +
  '                                '                             +  .
    Motivation A                                                _______
        .-''''''-.     Motivation B                            '.__  _.'
      .'   +      '.'''''.                                       |;  |
      |  +      .' .+     '.                                     |;  |
      |        | '          \                                    |'  |
      '.       |            |                                    |   |
       ''.''''''. _.----._.'      === [[ FORMULA ]] ==>        .'     '.
       .' +      '.       \                                   |---.___  \
      | '         |        \                                 |        '__\
      |       +   |    +   |                                | +           \
      '.        .'..____.-'                                | ;             \
        ''''''''       Motivation C                       | ;   Solution    \
    Motivation D                                          ; ;               ;
                                                          '. '             .'
                                                            '-------------'
*/


typedef struct Solution Solution;
typedef float (*solution_formula)(const Solution *);

// Each solution has a set of formulas, those formulas read the compounds data
// and combine them together to fulfill the motivations
// clang-format off
#define SOLUTION_FORMULAS_DEF(Label)                                               \
  static inline float solution_##Label##_get_epic_meaning(const Solution *);       \
  static inline float solution_##Label##_get_empowerment(const Solution *);        \
  static inline float solution_##Label##_get_social_influence(const Solution *);   \
  static inline float solution_##Label##_get_unpredictability(const Solution *);   \
  static inline float solution_##Label##_get_avoidance(const Solution *);          \
  static inline float solution_##Label##_get_scarcity(const Solution *);           \
  static inline float solution_##Label##_get_ownership(const Solution *);          \
  static inline float solution_##Label##_get_accomplishment(const Solution *);     \
  static inline float solution_##Label##_get_excitment(const Solution *);          \
  static inline float solution_##Label##_get_social_bounding(const Solution *);    \
  static inline float solution_##Label##_get_friction(const Solution *);           \
  static inline float solution_##Label##_get_reward(const Solution *);
// clang-format on

typedef enum {
  SolutionType_Module,
  SolutionType_Persona,
  SolutionType_COUNT,
} SolutionType;

struct Solution {

  alloc_id id;
  SolutionType type;

  union {
    SolutionModule module;
    SolutionPersona persona;
  };
};

static inline void solution_module_init(Solution *solution,
                                        const SolutionModule *module) {
  solution->type = SolutionType_Module;
  memcpy(&solution->module, module, sizeof(SolutionModule));
}

static inline void solution_persona_init(Solution *solution,
                                         const SolutionPersona *persona) {
  solution->type = SolutionType_Persona;
  memcpy(&solution->persona, persona, sizeof(SolutionPersona));
}

#endif
