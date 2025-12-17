#ifndef _EMMA_DATASET_SOCIAL_H_
#define _EMMA_DATASET_SOCIAL_H_

typedef struct {
  // outcome can be shared accross peers
  bool shareable;
  // include group based interactions (like, comment...)
  bool collaborative;
  // pits peers against each others
  bool competitive;
  // showcase outcome on user profile or leaderboard (status, score, badges...)
  bool published;
} DataSetSocial;

/*
  Higher if the module publish others results and outcome, may
  influence user to follow the movement. Also considers user’s ability to share,
  as it may encourage them to show their best outcome to others.
 */
static inline float dataset_social_get_influence(const DataSetSocial *set) {

  static const float w_publish = 0.7;
  static const float w_share = 0.3;

  return w_publish * set->published + w_share * set->shareable;
}

/*
  Mostly based on module’s collaborative nature. With a strong malus if
  competitive.
 */
static inline float dataset_social_get_bounding(const DataSetSocial *set) {

  static const float w_compet = 0.7;
  static const float w_collab = 0.3;

  return w_compet * set->competitive + w_collab * set->collaborative;
}

/*
  Avoidance is being triggered by fear of missing opportunities, which may be
  influenced by seeing others succeeding, being praised or getting high score
  and benefits. Thus, the formula mostly relies on the module ability to expose
  user’s outcome to public. Some competitiveness might be involved as well, for
  higher competition means higher fear of loss.
 */
static inline float dataset_social_get_avoidance(const DataSetSocial *set) {

  static const float w_publish = 0.8;
  static const float w_compet = 0.2;

  return w_publish * set->published + w_compet * set->competitive;
}

/*
  Both collaboration and competitiveness mostly drive increasing user’s
  accomplishment feeling by providing them the sensation of achieving something
  may it be with or against people. Accomplishment are also meant to be shared,
  hence the shareable and public also plays a lighter role in it.
 */
static inline float
dataset_social_get_accomplishment(const DataSetSocial *set) {

  static const float w_publish = 0.15;
  static const float w_share = 0.15;
  static const float w_collab = 0.3;
  static const float w_compet = 0.4;

  // clang-format off
  return   w_publish * set->published
         + w_share   * set->shareable
         + w_collab  * set->collaborative
         + w_compet  * set->competitive;
  // clang-format on
}

#endif
