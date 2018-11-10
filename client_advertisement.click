/*
 * Broadcast client address 
 * Protocol identifier for this frame is 07 00
 */

InfiniteSource(DATA \<
  // Ethernet header
  ff:ff:ff:ff:ff:ff e0:2a:82:43:5b:e2 07 00 c0 a8 2a 05
>, LIMIT 1, STOP true) 
-> Print (Ping:)
-> Queue
-> ToDevice(wlp1s0);
