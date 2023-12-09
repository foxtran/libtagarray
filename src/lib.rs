/// A multi-dimensional type-tagged container.
///
/// Usage: Record()

use pyo3::prelude::*;

mod container;
mod record;

pub use container::Container;
pub use record::Record;

/// Formats the sum of two numbers as string.
#[pyfunction]
fn sum_as_string(a: usize, b: usize) -> PyResult<String> {
    Ok((a + b).to_string())
}

/// A Python module implemented in Rust.
#[pymodule]
fn libtagarray(py: Python<'_>, m: &PyModule) -> PyResult<()> {
    m.add_function(wrap_pyfunction!(sum_as_string, m)?)?;
    m.add_class::<Record>()?;
    m.add_class::<Container>()?;

    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        let result = sum_as_string(2, 2);
        assert!(result.is_ok());
        assert_eq!(result.unwrap(), String::from("4"));
    }
}
