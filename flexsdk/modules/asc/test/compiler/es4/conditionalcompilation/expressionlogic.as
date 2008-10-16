package {
  CONFIG const nondebug= !CONFIG::debug
  print(CONFIG::debug)
  print(CONFIG::nondebug)
  CONFIG::debug function log(msg:String) { print(msg); }
  CONFIG::nondebug function log(msg:String) {}

  log("test message")
}