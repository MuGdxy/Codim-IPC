#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>

namespace py = pybind11;
namespace JGSL {

//NOTE: squared distance

template <class T, int dim>
void Point_Edge_Distance(const Eigen::Matrix<T, dim, 1>& p, 
    const Eigen::Matrix<T, dim, 1>& e0, 
    const Eigen::Matrix<T, dim, 1>& e1, 
    T& dist2)
{
    if constexpr (dim == 2) {
        const Eigen::Matrix<T, 2, 1> e = e1 - e0;
        T numerator = (e[1] * p[0] - e[0] * p[1] + e1[0] * e0[1] - e1[1] * e0[0]);
        dist2 = numerator * numerator / e.squaredNorm();
    }
    else {
        dist2 = (e0 - p).cross(e1 - p).squaredNorm() / (e1 - e0).squaredNorm();
    }
}

template <class T>
void g_PE2D(T v01, T v02, T v11, T v12, T v21, T
            v22, T g[6])
{
  T t13;
  T t14;
  T t23;
  T t25;
  T t24;
  T t26;
  T t27;

  /* G_PE2D */
  /*     G = G_PE2D(V01,V02,V11,V12,V21,V22) */
  /*     This function was generated by the Symbolic Math Toolbox version 8.3. */
  /*     16-Mar-2020 15:56:29 */
  t13 = -v21 + v11;
  t14 = -v22 + v12;
  t23 = 1.0 / (t13 * t13 + t14 * t14);
  t25 = ((v11 * v22 + -(v12 * v21)) + t14 * v01) + -(t13 * v02);
  t24 = t23 * t23;
  t26 = t25 * t25;
  t27 = (v11 * 2.0 + -(v21 * 2.0)) * t24 * t26;
  t26 *= (v12 * 2.0 + -(v22 * 2.0)) * t24;
  g[0] = t14 * t23 * t25 * 2.0;
  g[1] = t13 * t23 * t25 * -2.0;
  t24 = t23 * t25;
  g[2] = -t27 - t24 * (-v22 + v02) * 2.0;
  g[3] = -t26 + t24 * (-v21 + v01) * 2.0;
  g[4] = t27 + t24 * (v02 - v12) * 2.0;
  g[5] = t26 - t24 * (v01 - v11) * 2.0;
}

template <class T>
void g_PE3D(T v01, T v02, T v03, T v11, T v12, T v13,
    T v21, T v22, T v23, T g[9])
{
    T t17;
    T t18;
    T t19;
    T t20;
    T t21;
    T t22;
    T t23;
    T t24;
    T t25;
    T t42;
    T t44;
    T t45;
    T t46;
    T t43;
    T t50;
    T t51;
    T t52;

    /* G_PE */
    /*     G = G_PE(V01,V02,V03,V11,V12,V13,V21,V22,V23) */
    /*     This function was generated by the Symbolic Math Toolbox version 8.3. */
    /*     10-Jun-2019 18:02:37 */
    t17 = -v11 + v01;
    t18 = -v12 + v02;
    t19 = -v13 + v03;
    t20 = -v21 + v01;
    t21 = -v22 + v02;
    t22 = -v23 + v03;
    t23 = -v21 + v11;
    t24 = -v22 + v12;
    t25 = -v23 + v13;
    t42 = 1.0 / ((t23 * t23 + t24 * t24) + t25 * t25);
    t44 = t17 * t21 + -(t18 * t20);
    t45 = t17 * t22 + -(t19 * t20);
    t46 = t18 * t22 + -(t19 * t21);
    t43 = t42 * t42;
    t50 = (t44 * t44 + t45 * t45) + t46 * t46;
    t51 = (v11 * 2.0 + -(v21 * 2.0)) * t43 * t50;
    t52 = (v12 * 2.0 + -(v22 * 2.0)) * t43 * t50;
    t43 = (v13 * 2.0 + -(v23 * 2.0)) * t43 * t50;
    g[0] = t42 * (t24 * t44 * 2.0 + t25 * t45 * 2.0);
    g[1] = -t42 * (t23 * t44 * 2.0 - t25 * t46 * 2.0);
    g[2] = -t42 * (t23 * t45 * 2.0 + t24 * t46 * 2.0);
    g[3] = -t51 - t42 * (t21 * t44 * 2.0 + t22 * t45 * 2.0);
    g[4] = -t52 + t42 * (t20 * t44 * 2.0 - t22 * t46 * 2.0);
    g[5] = -t43 + t42 * (t20 * t45 * 2.0 + t21 * t46 * 2.0);
    g[6] = t51 + t42 * (t18 * t44 * 2.0 + t19 * t45 * 2.0);
    g[7] = t52 - t42 * (t17 * t44 * 2.0 - t19 * t46 * 2.0);
    g[8] = t43 - t42 * (t17 * t45 * 2.0 + t18 * t46 * 2.0);
}

template <class T, int dim>
void Point_Edge_Distance_Gradient(const Eigen::Matrix<T, dim, 1>& p, 
    const Eigen::Matrix<T, dim, 1>& e0, 
    const Eigen::Matrix<T, dim, 1>& e1, 
    Eigen::Matrix<T, dim * 3, 1>& grad)
{
    if constexpr (dim == 2) {
        g_PE2D(p[0], p[1], e0[0], e0[1], e1[0], e1[1], grad.data());
    }
    else {
        g_PE3D(p[0], p[1], p[2],
            e0[0], e0[1], e0[2],
            e1[0], e1[1], e1[2],
            grad.data());
    }
}

template <class T>
void H_PE2D(T v01, T v02, T v11, T v12, T v21, T
            v22, T H[36])
{
  T t15;
  T t16;
  T t17;
  T t18;
  T t19;
  T t20;
  T t21;
  T t22;
  T t23;
  T t24;
  T t31;
  T t34;
  T t32;
  T t33;
  T t35;
  T t60;
  T t59;
  T t62;
  T t64;
  T t65;
  T t68;
  T t71;
  T t72;
  T t75;
  T t76;
  T t77;
  T t78;
  T t79;
  T t90;
  T t92;
  T t94;
  T t96;
  T t99;
  T t93;
  T t97;
  T t98;
  T t100;
  T t102_tmp;

  /* H_PE2D */
  /*     H = H_PE2D(V01,V02,V11,V12,V21,V22) */
  /*     This function was generated by the Symbolic Math Toolbox version 8.3. */
  /*     16-Mar-2020 15:56:29 */
  t15 = -v11 + v01;
  t16 = -v12 + v02;
  t17 = -v21 + v01;
  t18 = -v22 + v02;
  t19 = -v21 + v11;
  t20 = -v22 + v12;
  t21 = v11 * 2.0 + -(v21 * 2.0);
  t22 = v12 * 2.0 + -(v22 * 2.0);
  t23 = t19 * t19;
  t24 = t20 * t20;
  t31 = 1.0 / (t23 + t24);
  t34 = ((v11 * v22 + -(v12 * v21)) + t20 * v01) + -(t19 * v02);
  t32 = t31 * t31;
  t33 = pow(t31, 3.0);
  t35 = t34 * t34;
  t60 = t31 * t34 * 2.0;
  t59 = -(t19 * t20 * t31 * 2.0);
  t62 = t32 * t35 * 2.0;
  t64 = t21 * t21 * t33 * t35 * 2.0;
  t65 = t22 * t22 * t33 * t35 * 2.0;
  t68 = t15 * t21 * t32 * t34 * 2.0;
  t71 = t16 * t22 * t32 * t34 * 2.0;
  t72 = t17 * t21 * t32 * t34 * 2.0;
  t75 = t18 * t22 * t32 * t34 * 2.0;
  t76 = t19 * t21 * t32 * t34 * 2.0;
  t77 = t20 * t21 * t32 * t34 * 2.0;
  t78 = t19 * t22 * t32 * t34 * 2.0;
  t79 = t20 * t22 * t32 * t34 * 2.0;
  t90 = t21 * t22 * t33 * t35 * 2.0;
  t92 = t16 * t20 * t31 * 2.0 + t77;
  t94 = -(t17 * t19 * t31 * 2.0) + t78;
  t96 = (t18 * t19 * t31 * 2.0 + -t60) + t76;
  t99 = (-(t15 * t20 * t31 * 2.0) + -t60) + t79;
  t93 = t15 * t19 * t31 * 2.0 + -t78;
  t35 = -(t18 * t20 * t31 * 2.0) + -t77;
  t97 = (t17 * t20 * t31 * 2.0 + t60) + -t79;
  t98 = (-(t16 * t19 * t31 * 2.0) + t60) + -t76;
  t100 = ((-(t15 * t16 * t31 * 2.0) + t71) + -t68) + t90;
  t19 = ((-(t17 * t18 * t31 * 2.0) + t75) + -t72) + t90;
  t102_tmp = t17 * t22 * t32 * t34;
  t76 = t15 * t22 * t32 * t34;
  t22 = (((-(t15 * t17 * t31 * 2.0) + t62) + -t65) + t76 * 2.0) + t102_tmp * 2.0;
  t33 = t18 * t21 * t32 * t34;
  t20 = t16 * t21 * t32 * t34;
  t79 = (((-(t16 * t18 * t31 * 2.0) + t62) + -t64) + -(t20 * 2.0)) + -(t33 * 2.0);
  t77 = (((t15 * t18 * t31 * 2.0 + t60) + t68) + -t75) + -t90;
  t78 = (((t16 * t17 * t31 * 2.0 + -t60) + t72) + -t71) + -t90;
  H[0] = t24 * t31 * 2.0;
  H[1] = t59;
  H[2] = t35;
  H[3] = t97;
  H[4] = t92;
  H[5] = t99;
  H[6] = t59;
  H[7] = t23 * t31 * 2.0;
  H[8] = t96;
  H[9] = t94;
  H[10] = t98;
  H[11] = t93;
  H[12] = t35;
  H[13] = t96;
  t35 = -t62 + t64;
  H[14] = (t35 + t18 * t18 * t31 * 2.0) + t33 * 4.0;
  H[15] = t19;
  H[16] = t79;
  H[17] = t77;
  H[18] = t97;
  H[19] = t94;
  H[20] = t19;
  t33 = -t62 + t65;
  H[21] = (t33 + t17 * t17 * t31 * 2.0) - t102_tmp * 4.0;
  H[22] = t78;
  H[23] = t22;
  H[24] = t92;
  H[25] = t98;
  H[26] = t79;
  H[27] = t78;
  H[28] = (t35 + t16 * t16 * t31 * 2.0) + t20 * 4.0;
  H[29] = t100;
  H[30] = t99;
  H[31] = t93;
  H[32] = t77;
  H[33] = t22;
  H[34] = t100;
  H[35] = (t33 + t15 * t15 * t31 * 2.0) - t76 * 4.0;
}

template <class T>
void H_PE3D(T v01, T v02, T v03, T v11, T v12, T v13,
    T v21, T v22, T v23, T H[81])
{
    T t17;
    T t18;
    T t19;
    T t20;
    T t21;
    T t22;
    T t23;
    T t24;
    T t25;
    T t26;
    T t27;
    T t28;
    T t35;
    T t36;
    T t37;
    T t50;
    T t51;
    T t52;
    T t53;
    T t54;
    T t55;
    T t56;
    T t62;
    T t70;
    T t71;
    T t75;
    T t79;
    T t80;
    T t84;
    T t88;
    T t38;
    T t39;
    T t40;
    T t41;
    T t42;
    T t43;
    T t44;
    T t46;
    T t48;
    T t57;
    T t58;
    T t60;
    T t63;
    T t65;
    T t67;
    T t102;
    T t103;
    T t104;
    T t162;
    T t163;
    T t164;
    T t213;
    T t214;
    T t215;
    T t216;
    T t217;
    T t218;
    T t225;
    T t226;
    T t227;
    T t229;
    T t230;
    T t311;
    T t231;
    T t232;
    T t233;
    T t234;
    T t235;
    T t236;
    T t237;
    T t238;
    T t239;
    T t240;
    T t245;
    T t279;
    T t281;
    T t282;
    T t283;
    T t287;
    T t289;
    T t247;
    T t248;
    T t249;
    T t250;
    T t251;
    T t252;
    T t253;
    T t293;
    T t295;
    T t299;
    T t300;
    T t303;
    T t304;
    T t294;
    T t297;
    T t301;
    T t302;

    /* H_PE */
    /*     H = H_PE(V01,V02,V03,V11,V12,V13,V21,V22,V23) */
    /*     This function was generated by the Symbolic Math Toolbox version 8.3. */
    /*     10-Jun-2019 18:02:39 */
    t17 = -v11 + v01;
    t18 = -v12 + v02;
    t19 = -v13 + v03;
    t20 = -v21 + v01;
    t21 = -v22 + v02;
    t22 = -v23 + v03;
    t23 = -v21 + v11;
    t24 = -v22 + v12;
    t25 = -v23 + v13;
    t26 = v11 * 2.0 + -(v21 * 2.0);
    t27 = v12 * 2.0 + -(v22 * 2.0);
    t28 = v13 * 2.0 + -(v23 * 2.0);
    t35 = t23 * t23;
    t36 = t24 * t24;
    t37 = t25 * t25;
    t50 = t17 * t21;
    t51 = t18 * t20;
    t52 = t17 * t22;
    t53 = t19 * t20;
    t54 = t18 * t22;
    t55 = t19 * t21;
    t56 = t17 * t20 * 2.0;
    t62 = t18 * t21 * 2.0;
    t70 = t19 * t22 * 2.0;
    t71 = t17 * t23 * 2.0;
    t75 = t18 * t24 * 2.0;
    t79 = t19 * t25 * 2.0;
    t80 = t20 * t23 * 2.0;
    t84 = t21 * t24 * 2.0;
    t88 = t22 * t25 * 2.0;
    t38 = t17 * t17 * 2.0;
    t39 = t18 * t18 * 2.0;
    t40 = t19 * t19 * 2.0;
    t41 = t20 * t20 * 2.0;
    t42 = t21 * t21 * 2.0;
    t43 = t22 * t22 * 2.0;
    t44 = t35 * 2.0;
    t46 = t36 * 2.0;
    t48 = t37 * 2.0;
    t57 = t50 * 2.0;
    t58 = t51 * 2.0;
    t60 = t52 * 2.0;
    t63 = t53 * 2.0;
    t65 = t54 * 2.0;
    t67 = t55 * 2.0;
    t102 = 1.0 / ((t35 + t36) + t37);
    t36 = t50 + -t51;
    t35 = t52 + -t53;
    t37 = t54 + -t55;
    t103 = t102 * t102;
    t104 = pow(t102, 3.0);
    t162 = -(t23 * t24 * t102 * 2.0);
    t163 = -(t23 * t25 * t102 * 2.0);
    t164 = -(t24 * t25 * t102 * 2.0);
    t213 = t18 * t36 * 2.0 + t19 * t35 * 2.0;
    t214 = t17 * t35 * 2.0 + t18 * t37 * 2.0;
    t215 = t21 * t36 * 2.0 + t22 * t35 * 2.0;
    t216 = t20 * t35 * 2.0 + t21 * t37 * 2.0;
    t217 = t24 * t36 * 2.0 + t25 * t35 * 2.0;
    t218 = t23 * t35 * 2.0 + t24 * t37 * 2.0;
    t35 = (t36 * t36 + t35 * t35) + t37 * t37;
    t225 = t17 * t36 * 2.0 + -(t19 * t37 * 2.0);
    t226 = t20 * t36 * 2.0 + -(t22 * t37 * 2.0);
    t227 = t23 * t36 * 2.0 + -(t25 * t37 * 2.0);
    t36 = t26 * t103;
    t229 = t36 * t213;
    t37 = t27 * t103;
    t230 = t37 * t213;
    t311 = t28 * t103;
    t231 = t311 * t213;
    t232 = t36 * t214;
    t233 = t37 * t214;
    t234 = t311 * t214;
    t235 = t36 * t215;
    t236 = t37 * t215;
    t237 = t311 * t215;
    t238 = t36 * t216;
    t239 = t37 * t216;
    t240 = t311 * t216;
    t214 = t36 * t217;
    t215 = t37 * t217;
    t216 = t311 * t217;
    t217 = t36 * t218;
    t245 = t37 * t218;
    t213 = t311 * t218;
    t279 = t103 * t35 * 2.0;
    t281 = t26 * t26 * t104 * t35 * 2.0;
    t282 = t27 * t27 * t104 * t35 * 2.0;
    t283 = t28 * t28 * t104 * t35 * 2.0;
    t287 = t26 * t27 * t104 * t35 * 2.0;
    t218 = t26 * t28 * t104 * t35 * 2.0;
    t289 = t27 * t28 * t104 * t35 * 2.0;
    t247 = t36 * t225;
    t248 = t37 * t225;
    t249 = t311 * t225;
    t250 = t36 * t226;
    t251 = t37 * t226;
    t252 = t311 * t226;
    t253 = t36 * t227;
    t35 = t37 * t227;
    t36 = t311 * t227;
    t293 = t102 * (t75 + t79) + t214;
    t295 = -(t102 * (t80 + t84)) + t213;
    t299 = t102 * ((t63 + t22 * t23 * 2.0) + -t60) + t217;
    t300 = t102 * ((t67 + t22 * t24 * 2.0) + -t65) + t245;
    t303 = -(t102 * ((t57 + t17 * t24 * 2.0) + -t58)) + t215;
    t304 = -(t102 * ((t60 + t17 * t25 * 2.0) + -t63)) + t216;
    t294 = t102 * (t71 + t75) + -t213;
    t297 = -(t102 * (t80 + t88)) + t35;
    t88 = -(t102 * (t84 + t88)) + -t214;
    t301 = t102 * ((t58 + t21 * t23 * 2.0) + -t57) + t253;
    t302 = t102 * ((t65 + t21 * t25 * 2.0) + -t67) + t36;
    t84 = t102 * ((t57 + t20 * t24 * 2.0) + -t58) + -t215;
    t80 = t102 * ((t60 + t20 * t25 * 2.0) + -t63) + -t216;
    t75 = -(t102 * ((t63 + t19 * t23 * 2.0) + -t60)) + -t217;
    t227 = -(t102 * ((t67 + t19 * t24 * 2.0) + -t65)) + -t245;
    t311 = ((-(t17 * t19 * t102 * 2.0) + t231) + -t232) + t218;
    t245 = ((-(t20 * t22 * t102 * 2.0) + t237) + -t238) + t218;
    t226 = ((-t102 * (t67 - t54 * 4.0) + t233) + t252) + -t289;
    t28 = ((-t102 * (t63 - t52 * 4.0) + t232) + -t237) + -t218;
    t27 = ((-t102 * (t58 - t50 * 4.0) + t247) + -t236) + -t287;
    t225 = ((-(t102 * (t65 + -(t55 * 4.0))) + t239) + t249) + -t289;
    t26 = ((-(t102 * (t60 + -(t53 * 4.0))) + t238) + -t231) + -t218;
    t103 = ((-(t102 * (t57 + -(t51 * 4.0))) + t250) + -t230) + -t287;
    t104 = (((-(t102 * (t56 + t62)) + t234) + t240) + t279) + -t283;
    t218 = (((-(t102 * (t56 + t70)) + t248) + t251) + t279) + -t282;
    t217 = (((-(t102 * (t62 + t70)) + -t229) + -t235) + t279) + -t281;
    t216 = t102 * (t71 + t79) + -t35;
    t215 = -(t102 * ((t58 + t18 * t23 * 2.0) + -t57)) + -t253;
    t214 = -(t102 * ((t65 + t18 * t25 * 2.0) + -t67)) + -t36;
    t213 = ((-(t17 * t18 * t102 * 2.0) + t230) + -t247) + t287;
    t37 = ((-(t20 * t21 * t102 * 2.0) + t236) + -t250) + t287;
    t36 = ((-(t18 * t19 * t102 * 2.0) + -t233) + -t249) + t289;
    t35 = ((-(t21 * t22 * t102 * 2.0) + -t239) + -t252) + t289;
    H[0] = t102 * (t46 + t48);
    H[1] = t162;
    H[2] = t163;
    H[3] = t88;
    H[4] = t84;
    H[5] = t80;
    H[6] = t293;
    H[7] = t303;
    H[8] = t304;
    H[9] = t162;
    H[10] = t102 * (t44 + t48);
    H[11] = t164;
    H[12] = t301;
    H[13] = t297;
    H[14] = t302;
    H[15] = t215;
    H[16] = t216;
    H[17] = t214;
    H[18] = t163;
    H[19] = t164;
    H[20] = t102 * (t44 + t46);
    H[21] = t299;
    H[22] = t300;
    H[23] = t295;
    H[24] = t75;
    H[25] = t227;
    H[26] = t294;
    H[27] = t88;
    H[28] = t301;
    H[29] = t299;
    H[30] = ((t235 * 2.0 + -t279) + t281) + t102 * (t42 + t43);
    H[31] = t37;
    H[32] = t245;
    H[33] = t217;
    H[34] = t27;
    H[35] = t28;
    H[36] = t84;
    H[37] = t297;
    H[38] = t300;
    H[39] = t37;
    H[40] = ((t251 * -2.0 + -t279) + t282) + t102 * (t41 + t43);
    H[41] = t35;
    H[42] = t103;
    H[43] = t218;
    H[44] = t226;
    H[45] = t80;
    H[46] = t302;
    H[47] = t295;
    H[48] = t245;
    H[49] = t35;
    H[50] = ((t240 * -2.0 + -t279) + t283) + t102 * (t41 + t42);
    H[51] = t26;
    H[52] = t225;
    H[53] = t104;
    H[54] = t293;
    H[55] = t215;
    H[56] = t75;
    H[57] = t217;
    H[58] = t103;
    H[59] = t26;
    H[60] = ((t229 * 2.0 + -t279) + t281) + t102 * (t39 + t40);
    H[61] = t213;
    H[62] = t311;
    H[63] = t303;
    H[64] = t216;
    H[65] = t227;
    H[66] = t27;
    H[67] = t218;
    H[68] = t225;
    H[69] = t213;
    H[70] = ((t248 * -2.0 + -t279) + t282) + t102 * (t38 + t40);
    H[71] = t36;
    H[72] = t304;
    H[73] = t214;
    H[74] = t294;
    H[75] = t28;
    H[76] = t226;
    H[77] = t104;
    H[78] = t311;
    H[79] = t36;
    H[80] = ((t234 * -2.0 + -t279) + t283) + t102 * (t38 + t39);
}

template <class T, int dim>
void Point_Edge_Distance_Hessian(const Eigen::Matrix<T, dim, 1>& p, 
    const Eigen::Matrix<T, dim, 1>& e0, 
    const Eigen::Matrix<T, dim, 1>& e1, 
    Eigen::Matrix<T, dim * 3, dim * 3>& Hessian)
{
    if constexpr (dim == 2) {
        H_PE2D(p[0], p[1], e0[0], e0[1], e1[0], e1[1], Hessian.data());
    }
    else {
        H_PE3D(p[0], p[1], p[2],
            e0[0], e0[1], e0[2],
            e1[0], e1[1], e1[2],
            Hessian.data());
    }
}

}