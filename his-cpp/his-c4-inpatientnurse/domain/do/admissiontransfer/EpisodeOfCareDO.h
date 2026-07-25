#pragma once
#ifndef _EPISODEOFCAREDO_
#define _EPISODEOFCAREDO_

#include "../DoInclude.h"

class EpisodeOfCareDO : public BaseDO
{
public:
    EpisodeOfCareDO() : BaseDO("adm_episode_of_care") {}
};

typedef std::shared_ptr<EpisodeOfCareDO> PtrEpisodeOfCareDO;

#endif
