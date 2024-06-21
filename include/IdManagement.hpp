/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-06-21 13:18:14
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-06-21 15:46:45
 * @FilePath: /cplusplus/submodule/algorithmBase/include/IdManagement.hpp
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#pragma once
#include <array>
#include <cstdint>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace algorithmBase {

template <std::size_t MaxID>
class IdManagement {
public:
    static constexpr uint16_t invalidID = std::numeric_limits<uint16_t>::max();
    enum class IdState {
        NOT_USE,
        IN_USE,
        DELETED,
    };
    void init() {
        for (auto& id : mIdArray) {
            id = IdState::NOT_USE;
        }
        mIdArray[0] = IdState::IN_USE;
        isInit = true;
    }
    uint16_t getNewId() {
        uint16_t id = invalidID;
        int count{};
        if (isInit) {
            while ((IdState::NOT_USE != mIdArray[mCurId]) && ((mMaxId + 1) > count)) {
                mCurId = (mCurId + 1u) % (mMaxId + 1);
                count++;
            }

            if (IdState::NOT_USE == mIdArray[mCurId]) {
                mIdArray[mCurId] = IdState::IN_USE;
                id = mCurId;
                mCurId = ((mCurId + 1u) % (mMaxId + 1));
            }
        }

        return id;
    }
    void releaseId(uint16_t id) {
        if (0 < id && id <= mMaxId) {
            mIdArray[id] = IdState::DELETED;
        } else {
            throw std::out_of_range("IdManagement::releaseId id out of range");
        }
    }
    void onFinish() {
        for (auto& id : mIdArray) {
            if (id == IdState::DELETED) {
                id = IdState::NOT_USE;
            }
        }
    }

    void debug() {
        for (uint16_t i = 0; i <= mMaxId; i++) {
            std::string state;
            switch (mIdArray[i]) {
                case IdState::NOT_USE:
                    state = "NOT_USE";
                    break;
                case IdState::IN_USE:
                    state = "IN_USE";
                    break;
                case IdState::DELETED:
                    state = "DELETED";
                    break;
                default:
                    state = "UNKNOWN";
                    break;
            }
            std::cout << i << " " << state << std::endl;
        }
    }

private:
    std::array<IdState, MaxID + 1> mIdArray;
    static constexpr uint16_t mMaxId{MaxID};
    uint16_t mCurId{0};
    bool isInit{false};
};
}  // namespace algorithmBase
