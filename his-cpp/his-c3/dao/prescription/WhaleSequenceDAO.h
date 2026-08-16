#pragma once

#ifndef _WHALE_SEQUENCE_DAO_
#define _WHALE_SEQUENCE_DAO_

#include "BaseDAO.h"

/**
 * Data access object for the `whale_sequence` business-sequence table.
 *
 * Provides atomic, thread-safe sequence generation per business key
 * by leveraging MySQL's `LAST_INSERT_ID(expr)` row-locking trick.
 *
 * Schema reference:
 *   CREATE TABLE `whale_sequence` (
 *     `seq_key`       VARCHAR(255) NOT NULL,
 *     `current_value` BIGINT       NOT NULL DEFAULT 0,
 *     `updated_at`    DATETIME(6)  NULL,
 *     PRIMARY KEY (`seq_key`)
 *   );
 *
 * Typical keys: "1:PR" (prescription), "1:OR" (order), "1:MD" (medical), ...
 */
class WhaleSequenceDAO : public BaseDAO
{
public:
	/**
	 * Atomically increment the counter for `seqKey` and return its new value.
	 *
	 * If `seqKey` does not yet exist, a row with current_value=0 is inserted
	 * first, so the call is always safe to invoke for never-seen keys.
	 *
	 * Concurrency: the UPDATE + SELECT LAST_INSERT_ID() pair runs on the same
	 * SqlSession connection. MySQL's row lock on the matched `seq_key` row
	 * serializes concurrent callers, so no two threads can get the same id.
	 *
	 * @param seqKey business key, e.g. "1:PR"
	 * @return the post-increment value (>= 1)
	 */
	uint64_t nextValue(const std::string& seqKey);
};

#endif // !_WHALE_SEQUENCE_DAO_
