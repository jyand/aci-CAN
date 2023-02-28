package main

import (
        "bufio"
        "fmt"
        "os"
        "regexp"
        "strconv"
        "strings"
)

const TOKEN string = " |#"
const DELIM string = ", "
const PAREN string = "\\(|\\)"

type CanMessage struct {
        timestamp float64
        id string
        data uint64
        dlc uint8
}

func CharReplace(s []string) []string {
        gre := regexp.MustCompile(TOKEN)
        re := regexp.MustCompile(PAREN)
        for k, v := range s {
                s[k] = string(gre.ReplaceAll([]byte(v), []byte(DELIM)))
                s[k] = string(re.ReplaceAll([]byte(s[k]), []byte("")))
        }
        return s
}

func DecomposeCanLog(s []string) []CanMessage {
        var canm []CanMessage
        for _, u := range s {
                str := strings.Split(u, DELIM)
                var temp CanMessage
                temp.timestamp, _ = strconv.ParseFloat(str[0], 64)
                temp.id = str[2]
                temp.data, _ = strconv.ParseUint(str[3], 16, 64)
                temp.dlc = SizeOf(str[3])
                canm = append(canm, temp)
        }
        return canm
}

func StringsFromFile(fpath string) []string {
        f, _ := os.Open(fpath)
        defer f.Close()
        var s []string
        sc := bufio.NewScanner(f)
        for sc.Scan() {
                s = append(s, sc.Text())
        }
        return s
}

func FileFromStrings(fname string, s []string) {
        f, _ := os.Create(fname)
        defer f.Close()
        for _, u := range s {
                f.Write([]byte(u))
                f.WriteString("\n")
        }
}

func SizeOf(s string) uint8 {
        size := len(strings.Split(s, ""))
        return uint8(size/2 + size%2)
}

func Echo(s []string) {
        for _, v := range s {
                fmt.Println(v)
        }
}

func ComposeCanCsv(msg []CanMessage) []string {
        var str []string
        for _, u := range msg {
                var temp []string
                temp = append(temp, strconv.FormatUint(uint64(u.timestamp), 10), u.id, strconv.FormatUint(u.data, 16), strconv.FormatUint(uint64(u.dlc), 10))
                str = append(str, strings.Join(temp, ", "))
        }
        return str
}

func main() {
        lines := StringsFromFile(os.Args[1])
        lines = CharReplace(lines)
        Echo(lines)
        FileFromStrings(os.Args[2], ComposeCanCsv(DecomposeCanLog(lines)))
}
