class HRecord {
private:
  const int32_t type_id_;
  const int32_t itemsize_; // size of one element in bytes
  int64_t count_;          // number of elements
  int64_t ndims_;
  std::vector<int64_t> dims_;
  std::shared_ptr<uint8_t[]> data_;
  std::string description_;

public:
  HRecord(const int32_t type_id, std::vector<int64_t> dimensions,
          const uint8_t *const data = nullptr,
          const std::string &description = std::string(""))
      : type_id_(type_id), itemsize_(utils::get_storage_size(type_id)),
        ndims_(dimensions.size()), dims_(dimensions),
        description_(description) {
    this->count_ =
        std::accumulate(std::begin(this->dims_), std::end(this->dims_), 1,
                        std::multiplies<int64_t>());
    assert(this->byte_count() != 0);
    if (this->byte_count() <= 0)
      return;
    std::shared_ptr<uint8_t[]> tmp(new (std::align_val_t(64), std::nothrow)
                                       uint8_t[this->byte_count()],
                                   std::default_delete<uint8_t[]>());
    this->data_ = tmp;
    if (this->data_.get() == nullptr) {
      return;
    }
    if (data == nullptr) {
      std::fill(this->data_.get(), this->data_.get() + this->byte_count(), 0);
    } else {
      std::copy(data, data + this->byte_count(), this->data_.get());
    }
  }

  HRecord(const HRecord &rec)
      : type_id_(rec.type_id()), itemsize_(rec.itemsize()), count_(rec.count()),
        ndims_(rec.ndims()), dims_(rec.shape()), data_(rec.data_),
        description_(rec.description()) {}

  ~HRecord() {}

  inline const std::string &description() const noexcept {
    return this->description_;
  }

  inline std::string &description() noexcept { return this->description_; }

  inline void description(const std::string &str) noexcept {
    this->description_ = str;
  }

  inline int32_t type_id() const noexcept { return this->type_id_; }

  inline int32_t itemsize() const noexcept { return this->itemsize_; }

  inline int64_t count() const noexcept { return this->count_; }

  inline int64_t byte_count() const noexcept {
    return this->count_ * this->itemsize_;
  }

  inline int64_t ndims() const noexcept { return this->ndims_; }

  inline const std::vector<int64_t> &shape() const noexcept {
    return this->dims_;
  }

  inline void shape(const std::vector<int64_t> &shape) noexcept {
    int64_t new_count = std::accumulate(std::begin(shape), std::end(shape), 1,
                                        std::multiplies<int64_t>());
    assert(new_count == this->count_);
    this->ndims_ = shape.size();
    this->dims_ = shape;
  }

  template <typename T> inline auto raw_data() const noexcept {
    return reinterpret_cast<T>(this->data_.get());
  }

  inline void expand(const std::vector<int64_t> &shape) noexcept {
    assert(this->ndims_ == 1);
    assert(shape.size() == 1);
    int64_t new_count = std::accumulate(std::begin(shape), std::end(shape), 1,
                                        std::multiplies<int64_t>());
    if (new_count == this->count_)
      return;
    assert(new_count > this->count_);
    std::shared_ptr<uint8_t[]> tmp(new (std::align_val_t(64), std::nothrow)
                                       uint8_t[this->itemsize() * new_count],
                                   std::default_delete<uint8_t[]>());
    if (tmp.get() == nullptr) {
      return;
    }
    std::copy(this->data_.get(), this->data_.get() + this->byte_count(),
              tmp.get());
    this->count_ = new_count;
    this->ndims_ = shape.size();
    this->dims_ = shape;
    std::swap(this->data_, tmp);
  }

  inline std::shared_ptr<HRecord> copy() const noexcept {
    return std::make_shared<HRecord>(HRecord(*this));
  }

  inline std::shared_ptr<HRecord> deepcopy() const noexcept {
    return std::make_shared<HRecord>(HRecord(this->type_id(), this->shape(),
                                             this->data_.get(),
                                             this->description()));
  }

  inline RecordInfo info() const noexcept {
    RecordInfo recordInfo = {this->type_id_,
                             this->itemsize_,
                             this->count_,
                             this->ndims_,
                             const_cast<int64_t *>(this->dims_.data()),
                             this->data_.get(),
                             const_cast<char *>(this->description_.data())};
    return recordInfo;
  }

  inline static const HRecord invalid() noexcept {
    HRecord invalid(defines::TYPE_UNKNOWN, {-1}, nullptr, "invalid");
    return invalid;
  }
};
