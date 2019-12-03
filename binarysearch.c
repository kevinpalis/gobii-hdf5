function bsearch(i, lower, upper, target) {
  if (insert[i].mi == target)
    return insert[i].seq;
  if (insert[i].mi < target) {
    lower = i;                                                                                                                     i = floor((i+upper)/2);
    bsearch (i, lower, upper, target);
  }
  if (insert[i].mi > target) {
    upper = i;
    i = floor((i+lower)/2);
    bsearch (i, lower, upper, target);
  }
}
