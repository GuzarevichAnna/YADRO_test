#include <gtest/gtest.h>
#include <fstream>
#include <sstream>

std::string ExecuteCommand(const char* cmd) {
    std::array<char, 1024> buffer;
    std::string result;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}

void RunTest(const std::string& input, const std::string& expected_output) {
    std::ofstream test_file("../../test_file.txt");
    test_file << input;
    test_file.close();

    std::string command = "../../task ../../test_file.txt";
    std::string output = ExecuteCommand(command.c_str());

    EXPECT_EQ(output, expected_output);
}

TEST(CorrectnessTests, ExampleTest) {
    std::string input =
        "3\n"
        "09:00 19:00\n"
        "10\n"
        "08:48 1 client1\n"
        "09:41 1 client1\n"
        "09:48 1 client2\n"
        "09:52 3 client1\n"
        "09:54 2 client1 1\n"
        "10:25 2 client2 2\n"
        "10:58 1 client3\n"
        "10:59 2 client3 3\n"
        "11:30 1 client4\n"
        "11:35 2 client4 2\n"
        "11:45 3 client4\n"
        "12:33 4 client1\n"
        "12:43 4 client2\n"
        "15:52 4 client4\n";

    std::string expected_output =
        "09:00\n"
        "08:48 1 client1\n"
        "08:48 13 NotOpenYet\n"
        "09:41 1 client1\n"
        "09:48 1 client2\n"
        "09:52 3 client1\n"
        "09:52 13 ICanWaitNoLonger!\n"
        "09:54 2 client1 1\n"
        "10:25 2 client2 2\n"
        "10:58 1 client3\n"
        "10:59 2 client3 3\n"
        "11:30 1 client4\n"
        "11:35 2 client4 2\n"
        "11:35 13 PlaceIsBusy\n"
        "11:45 3 client4\n"
        "12:33 4 client1\n"
        "12:33 12 client4 1\n"
        "12:43 4 client2\n"
        "15:52 4 client4\n"
        "19:00 11 client3\n"
        "19:00\n"
        "1 70 05:58\n"
        "2 30 02:18\n"
        "3 90 08:01\n";

        RunTest(input, expected_output);
}

TEST(CorrectnessTests, UnusedTablesTest) {
    std::string input =
        "3\n"
        "09:00 19:00\n"
        "100\n"
        "10:00 1 client1\n"
        "10:30 2 client1 1\n"
        "11:30 4 client1\n";

    std::string expected_output =
        "09:00\n"
        "10:00 1 client1\n"
        "10:30 2 client1 1\n"
        "11:30 4 client1\n"
        "19:00\n"
        "1 100 01:00\n"
        "2 0 00:00\n"
        "3 0 00:00\n";

    RunTest(input, expected_output);
}

TEST(CorrectnessTests, WorkingHoursTest) {
    std::string input =
        "2\n"
        "09:00 17:00\n"
        "150\n"
        "08:59 1 client2\n"
        "09:01 1 client1\n"
        "16:59 4 client1\n"
        "17:01 1 client2\n";

    std::string expected_output =
        "09:00\n"
        "08:59 1 client2\n"
        "08:59 13 NotOpenYet\n"
        "09:01 1 client1\n"
        "16:59 4 client1\n"
        "17:01 1 client2\n"
        "17:01 13 NotOpenYet\n"
        "17:00\n"
        "1 0 00:00\n"
        "2 0 00:00\n";

    RunTest(input, expected_output);
}

TEST(CorrectnessTests, UnknownClientTest) {
    std::string input =
        "2\n"
        "10:00 18:00\n"
        "200\n"
        "10:05 1 client1\n"
        "11:05 2 client2 1\n"
        "11:15 2 client1 1\n"
        "12:05 4 client1\n"
        "12:30 4 client2\n";

    std::string expected_output =
        "10:00\n"
        "10:05 1 client1\n"
        "11:05 2 client2 1\n"
        "11:05 13 ClientUnknown\n"
        "11:15 2 client1 1\n"
        "12:05 4 client1\n"
        "12:30 4 client2\n"
        "12:30 13 ClientUnknown\n"
        "18:00\n"
        "1 200 00:50\n"
        "2 0 00:00\n";

    RunTest(input, expected_output);
}

TEST(CorrectnessTests, ClientQueueTest) {
    std::string input =
        "2\n"
        "10:00 18:00\n"
        "10\n"
        "10:00 1 client1\n"
        "10:05 2 client1 1\n"
        "11:00 1 client2\n"
        "11:05 2 client2 2\n"
        "12:00 1 client3\n"
        "12:05 3 client3\n"
        "13:00 1 client4\n"
        "13:05 3 client4\n"
        "13:30 1 client5\n"
        "13:35 3 client5\n"
        "14:00 4 client1\n"
        "15:00 4 client2\n";
    
    std::string expected_output =
        "10:00\n"
        "10:00 1 client1\n"
        "10:05 2 client1 1\n"
        "11:00 1 client2\n"
        "11:05 2 client2 2\n"
        "12:00 1 client3\n"
        "12:05 3 client3\n"
        "13:00 1 client4\n"
        "13:05 3 client4\n"
        "13:30 1 client5\n"
        "13:35 3 client5\n"
        "13:35 11 client5\n"
        "14:00 4 client1\n"
        "14:00 12 client3 1\n"
        "15:00 4 client2\n"
        "15:00 12 client4 2\n"
        "18:00 11 client3\n"
        "18:00 11 client4\n"
        "18:00\n"
        "1 80 07:55\n"
        "2 70 06:55\n";

    RunTest(input, expected_output);
}

TEST(CorrectnessTests, ChangingTables) {
    std::string input =
        "2\n"
        "10:00 18:00\n"
        "10\n"
        "10:00 1 client1\n"
        "10:00 2 client1 1\n"
        "11:00 1 client2\n"
        "11:00 2 client2 2\n"
        "12:00 2 client2 2\n"
        "13:00 2 client2 1\n"
        "14:00 4 client1\n"
        "15:00 2 client2 1\n";

    std::string expected_output =
        "10:00\n"
        "10:00 1 client1\n"
        "10:00 2 client1 1\n"
        "11:00 1 client2\n"
        "11:00 2 client2 2\n"
        "12:00 2 client2 2\n"
        "12:00 13 PlaceIsBusy\n"
        "13:00 2 client2 1\n"
        "13:00 13 PlaceIsBusy\n"
        "14:00 4 client1\n"
        "15:00 2 client2 1\n"
        "18:00 11 client2\n"
        "18:00\n"
        "1 70 07:00\n"
        "2 40 04:00\n";

    RunTest(input, expected_output);
}

TEST(ValidInputTests, ValidInputTest) {
    std::string input = "abcd\n";

    std::string expected_output = "abcd\n";

    RunTest(input, expected_output);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
