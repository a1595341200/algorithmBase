/*
 * @Author: yao.xie 1595341200@qq.com
 * @Date: 2024-03-11 15:04:28
 * @LastEditors: yao.xie 1595341200@qq.com
 * @LastEditTime: 2024-08-28 16:13:36
 * @FilePath: /cplusplus/submodule/algorithmBase/include/DynamicModel.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#ifndef DYNAMICMODEL_H
#define DYNAMICMODEL_H
#include <eigen3/Eigen/Dense>

/**
 * @brief The base class for dynamic models.
 *
 * This class provides an interface for dynamic models used in prediction algorithms.
 * Subclasses of this class should implement the `prediction` method to perform the prediction step.
 */
class DynamicModelBase {
public:
    /**
     * @brief Perform the prediction step of the dynamic model.
     *
     * This method updates the state vector `X` based on the given time step `dt`.
     *
     * @param X The state vector to be updated.
     * @param dt The time step for the prediction.
     */
    virtual void prediction(Eigen::VectorXd& X, Eigen::MatrixXd& P, float dt) = 0;

    virtual void update(Eigen::VectorXd& X [[maybe_unused]], Eigen::MatrixXd& P [[maybe_unused]],
                        const Eigen::VectorXd& Z [[maybe_unused]],
                        const Eigen::MatrixXd& R [[maybe_unused]]){};
    /**
     * @brief Default virtual destructor.
     */
    virtual ~DynamicModelBase() = default;
};

/**
 * @brief Represents a CV (Constant Velocity) dynamic model.
 *
 * This class inherits from the DynamicModelBase class and provides
 * functionality for predicting the state of a CV model.
 * x=[x,y,vx,vy]
 */
class CV : public DynamicModelBase {
public:
    /**
     * @brief Default constructor for the CV class.
     */
    CV();

    /**
     * @brief Predicts the state of the CV model.
     *
     * This function updates the state vector `X` based on the given time step `dt`.
     *
     * @param X The state vector to be updated.
     * @param dt The time step for prediction.
     */
    void prediction(Eigen::VectorXd& X, Eigen::MatrixXd& P, float dt) override;

private:
    Eigen::MatrixXd computationalA(float dt);

    Eigen::MatrixXd computationalQ(float dt);

    Eigen::MatrixXd A{};  ///< The state transition matrix.
    Eigen::MatrixXd Q{};  ///< The process noise covariance matrix.
    float mDt{};         ///< The time step for prediction.
    float Pq{0.01f};
};

/**
 * @brief Represents a continuous-time dynamic model.
 * x=[x,vx,y,vy]
 * X的最后一个元素代表w
 * 匀速转弯运动
 */
class CT : public DynamicModelBase {
public:
    CT();
    void prediction(Eigen::VectorXd& X, Eigen::MatrixXd& P, float dt) override;

private:
    Eigen::MatrixXd computationalA(float dt);
    Eigen::MatrixXd computationalQ(float dt);

    Eigen::MatrixXd A{};  ///< The state transition matrix.
    Eigen::MatrixXd Q{};  ///< The process noise covariance matrix.
    float mDt{};
    float w{};
    float Pw{0.01f};
};

/**
 * @brief The CTRA class represents a specific dynamic model called Constant Turn Rate and
 * Acceleration (CTRA).
 *
 * This class inherits from the DynamicModelBase class and provides an implementation for the
 * prediction method.
 */
class CTRA : public DynamicModelBase {
public:
    /**
     * @brief Predicts the state of the dynamic model after a given time interval.
     *
     * This method updates the state vector `X` based on the current state and the time interval
     * `dt`.
     *
     * @param X The state vector to be updated.
     * @param dt The time interval for prediction.
     */
    void prediction(Eigen::VectorXd& X, Eigen::MatrixXd& P, float dt) override;
};

/**
 * @brief The CA class represents a specific type of dynamic model.
 *
 * This class inherits from the DynamicModelBase class and provides
 * functionality for prediction using the CA model.
 * X[x,y,vx,vy,ax,ay]
 */
class CA : public DynamicModelBase {
public:
    /**
     * @brief Default constructor for the CA class.
     */
    CA();

    /**
     * @brief Performs prediction using the CA model.
     *
     * This function updates the state vector `X` based on the given time step `dt`.
     *
     * @param X The state vector to be updated.
     * @param dt The time step for prediction.
     */
    void prediction(Eigen::VectorXd& X, Eigen::MatrixXd& P, float dt) override;

    void update(Eigen::VectorXd& X, Eigen::MatrixXd& P, const Eigen::VectorXd& Z,
                const Eigen::MatrixXd& R) override;

    Eigen::MatrixXd A{Eigen::MatrixXd::Zero(6, 6)};     ///< The state transition matrix.
    Eigen::MatrixXd Q{Eigen::MatrixXd::Zero(6, 6)};     ///< The process noise covariance matrix.
    Eigen::MatrixXd Pacc{};  ///< The process noise covariance matrix.
    Eigen::MatrixXd H{};

private:
    Eigen::MatrixXd computationalA(float dt);

    Eigen::MatrixXd computationalQ(float dt);

    float mDt{0};           ///< The time step for prediction.
};

#endif /* DYNAMICMODEL_H */
