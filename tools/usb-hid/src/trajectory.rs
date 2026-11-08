// SPDX-License-Identifier: GPL-2.0-only
//! 二维位移缩放、旋转和整数余量累计。
#[derive(Debug, Clone, Copy, Default, PartialEq)]
pub struct Vector {
    pub x: f64,
    pub y: f64,
}
#[derive(Debug, Clone, Copy)]
pub struct Transform {
    pub matrix: [[f64; 2]; 2],
}
impl Default for Transform {
    fn default() -> Self {
        Self { matrix: [[1., 0.], [0., 1.]] }
    }
}
impl Transform {
    pub fn scale(x: f64, y: f64) -> Option<Self> {
        if !x.is_finite() || !y.is_finite() || x.abs() > 64. || y.abs() > 64. {
            None
        } else {
            Some(Self { matrix: [[x, 0.], [0., y]] })
        }
    }
    pub fn rotate(radians: f64) -> Option<Self> {
        if !radians.is_finite() {
            return None;
        }
        let (s, c) = radians.sin_cos();
        Some(Self { matrix: [[c, -s], [s, c]] })
    }
    pub fn apply(&self, v: Vector) -> Vector {
        Vector {
            x: self.matrix[0][0] * v.x + self.matrix[0][1] * v.y,
            y: self.matrix[1][0] * v.x + self.matrix[1][1] * v.y,
        }
    }
}
#[derive(Debug, Clone, Default)]
pub struct Remainder {
    pub x: f64,
    pub y: f64,
}
impl Remainder {
    pub fn quantize(&mut self, v: Vector) -> Option<(i32, i32)> {
        let x = v.x + self.x;
        let y = v.y + self.y;
        if !x.is_finite()
            || !y.is_finite()
            || x.abs() > i32::MAX as f64
            || y.abs() > i32::MAX as f64
        {
            return None;
        }
        let ix = x.trunc() as i32;
        let iy = y.trunc() as i32;
        self.x = x - f64::from(ix);
        self.y = y - f64::from(iy);
        Some((ix, iy))
    }
}
