#ifndef _EMMA_SOLUTION_CORE_H_
#define _EMMA_SOLUTION_CORE_H_

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
      Element A                                                 _______
        .-''''''-.     Element B                               '.__  _.'
      .'   +      '.'''''.                                       |;  |
      |  +      .' .+     '.                                     |;  |
      |        | '          \                                    |'  |
      '.       |            |                                    |   |
       ''.''''''. _.----._.'      === [[ FORMULA ]] ==>        .'     '.
       .' +      '.       \                                   |---.___  \
      | '         |        \                                 |        '__\
      |       +   |    +   |                                | +           \
      '.        .'..____.-'                                | ;             \
        ''''''''       Element C                          | ;   Solution    \
    Element D                                             ; ;               ;
                                                          '. '             .'
                                                            '-------------'
*/

typedef struct {

  // core drive
  float epic_meaning;
  float empowerment;
  float social_influence;
  float unpredictability;
  float avoidance;
  float scarcity;
  float ownership;
  float accomplishment;

  // heatmap
  float excitment;
  float social_bounding;
  float friction;
  float reward;

} SolutionCore;

#endif
