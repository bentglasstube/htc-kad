package main

import (
  "bufio"
  "flag"
  "fmt"
  "os"
  "sort"
  "strconv"
)

type ByDist []Dist

func (a ByDist) Len() int { return len(a) }
func (a ByDist) Swap(i, j int) { a[i], a[j] = a[j], a[i] }
func (a ByDist) Less(i, j int) bool { return a[i].distance < a[j].distance }

type Dist struct {
  value uint64
  distance uint64
}

func nearest(key uint64, list []uint64, count uint) [] uint64 {
  var i uint

  mins := make([]Dist, 0, count)

  for i < count {
    d := Dist{ list[i], list[i] ^ key }
    mins = append(mins, d)
    i++
  }

  sort.Sort(ByDist(mins))

  for i := count; i < uint(len(list)); i++ {
    d := Dist{ list[i], list[i] ^ key }

    if d.distance < mins[count - 1].distance {
      i := count - 1
      for ; i > 0 && mins[i - 1].distance > d.distance; i-- {
        mins[i] = mins[i - 1]
      }
      mins[i] = d;
    }
  }

  vals := make([]uint64, 0, count)
  for i := uint(0); i < count; i++ {
    vals = append(vals, mins[i].value)
  }

  return vals
}

func main() {
  var request_size uint

  flag.UintVar(&request_size, "request_size", 1, "Number of resutls to return")
  flag.Parse()

  list := make([] uint64, 0)

  scanner := bufio.NewScanner(os.Stdin)
  for scanner.Scan() {
    line := scanner.Text();
    if line == "" {
      break
    }

    u, err := strconv.ParseUint(line, 10, 64)
    if err != nil {
      break
    }

    list = append(list, u)
  }

  for scanner.Scan() {
    line := scanner.Text();
    u, err := strconv.ParseUint(line, 10, 64)
    if err != nil {
      break
    }

    vals := nearest(u, list, request_size)
    for i := 0; i < len(vals); i++ {
      fmt.Println(vals[i])
    }
  }
}
