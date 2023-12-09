use crate::Record;
use std::collections::HashMap;
use pyo3::pyclass;

#[pyclass]
pub struct Container {
    version: i32,
    comment: String,
    records: HashMap<String, Record>,
}
