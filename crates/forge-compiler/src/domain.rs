use std::fmt;

#[derive(Clone, Debug, PartialEq)]
pub enum ValueWidth {
    FixedByteSize(usize),
    PointerSize,
    Auto,
}

fn write_value_width_suffix(
    f: &mut fmt::Formatter<'_>,
    width: &ValueWidth,
    type_suffix: &str,
) -> fmt::Result {
    match width {
        ValueWidth::FixedByteSize(width) => {
            write!(f, "{}{}", type_suffix, width * 8)
        }
        ValueWidth::PointerSize => write!(f, "{}size", type_suffix),
        ValueWidth::Auto => Ok(()),
    }
}

#[derive(Clone, Debug, PartialEq)]
pub struct ValueInt<IntType> {
    pub value: IntType,
    pub width: ValueWidth,
}

pub type ValueIntSigned = ValueInt<i64>;

impl fmt::Display for ValueIntSigned {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.value)?;
        write_value_width_suffix(f, &self.width, "i")
    }
}

pub type ValueIntUnsigned = ValueInt<u64>;

impl fmt::Display for ValueIntUnsigned {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.value)?;
        write_value_width_suffix(f, &self.width, "u")
    }
}

#[derive(Clone, Debug, PartialEq)]
pub struct ValueFloat {
    pub value: f64,
    pub width: ValueWidth,
}

impl fmt::Display for ValueFloat {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.value)?;
        write_value_width_suffix(f, &self.width, "f")
    }
}
