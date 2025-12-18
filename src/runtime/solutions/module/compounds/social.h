#ifndef _EMMA_MODULE_COMPOUND_SOCIAL_H_
#define _EMMA_MODULE_COMPOUND_SOCIAL_H_

typedef struct {
  // outcome can be shared accross peers
  bool shareable;
  // include group based interactions (like, comment...)
  bool collaborative;
  // pits peers against each others
  bool competitive;
  // showcase outcome on user profile or leaderboard (status, score, badges...)
  bool published;
} CompoundSocial;

/*
  Higher if the module publish others results and outcome, may
  influence user to follow the movement. Also considers user’s ability to share,
  as it may encourage them to show their best outcome to others.
 */
static inline float compound_social_get_influence(const CompoundSocial *set) {

  static const float w_publish = 0.7;
  static const float w_share = 0.3;

  return w_publish * set->published + w_share * set->shareable;
}

/*
  Mostly based on module’s collaborative nature. With a strong malus if
  competitive.
 */
static inline float compound_social_get_bounding(const CompoundSocial *set) {

  static const float w_compet = 0.7;
  static const float w_collab = 0.3;

  return w_compet * set->competitive + w_collab * set->collaborative;
}

/*
  Avoidance is being triggered by fear of missing opportunities, which may be
  influenced by seeing others succeeding, being praised or getting high score
  and benefits. Thus, the compound mostly relies on the module ability to expose
  user’s outcome to public. Some competitiveness might be involved as well, for
  higher competition means higher fear of loss.
 */
static inline float compound_social_get_avoidance(const CompoundSocial *set) {

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
compound_social_get_accomplishment(const CompoundSocial *set) {

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

/*
  In general, any social dynamic induce some kind of excitement:

  - Competing peers may provoke thrill by wanting to beat their score.
  - Doing a collaborative task is also exciting by creating something together.
  - Knowing that an action’s outcome will be published (on a feed, user-profile)
  also induce some kind of excitement as we know we may get recognition from it
  (comment, like, exposure)
 */
static inline float compound_social_get_excitement(const CompoundSocial *set) {

  static const float w_publish = 0.15;
  static const float w_collab = 0.35;
  static const float w_compet = 0.5;

  // clang-format off
  return   w_publish * set->published
         + w_collab  * set->collaborative
         + w_compet  * set->competitive;
  // clang-format on
}

#endif
