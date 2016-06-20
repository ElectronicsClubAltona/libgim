#include "hash/keccak.hpp"
#include "debug.hpp"
#include "tap.hpp"

#include <iomanip>


static constexpr
uint8_t
from_hex (char c) {
    switch (c) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return c - '0';

        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            return 10 + c - 'A';

        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return 10 + c - 'a';
    }

    unreachable ();
}


template <size_t S>
static constexpr
std::array<uint8_t,S/2>
make_array (const char (&str)[S])
{
    if (S < 1)
        return {};

    static_assert (S % 2 == 1, "requires full bytes + null");
    std::array<uint8_t,S/2> out {};
    
    for (size_t i = 0; i < S - 1; i+=2) {
        out[i / 2] = +from_hex (str[i]) << 4 | +from_hex (str[i+1]);
    }

    return out;
}


template <size_t S>
static
std::vector<uint8_t>
make_vector (const char (&str)[S]) {
    const auto arr = make_array (str);
    return std::vector<uint8_t> (arr.cbegin (), arr.cend ());
}


int
main (int, char**)
{
    util::TAP::logger tap;

    static const struct {
        const char *msg;
        std::vector<uint8_t> data;

        std::array<uint8_t, 28> sha3_224;
        std::array<uint8_t, 32> sha3_256;
        std::array<uint8_t, 48> sha3_384;
        std::array<uint8_t, 64> sha3_512;
    } TESTS[] = {
        {
            "  empty string",
            { },
            make_array ("6B4E03423667DBB73B6E15454F0EB1ABD4597F9A1B078E3F5B5A6BC7"),
            make_array ("A7FFC6F8BF1ED76651C14756A061D662F580FF4DE43B49FA82D80A4B80F8434A"),
            make_array ("0C63A75B845E4F7D01107D852E4C2485C51A50AAAA94FC61995E71BBEE983A2AC3713831264ADB47FB6BD1E058D5F004"),
            make_array ("A69F73CCA23A9AC5C8B567DC185A756E97C982164FE25859E0D1DCC1475C80A615B2123AF1F5F94C11E3E9402C3AC558F500199D95B6D3E301758586281DCD26"),
        },
        {
            "  8-bit string",
            make_vector ("CC"),
            make_array ("DF70ADC49B2E76EEE3A6931B93FA41841C3AF2CDF5B32A18B5478C39"),
            make_array ("677035391CD3701293D385F037BA32796252BB7CE180B00B582DD9B20AAAD7F0"),
            make_array ("5EE7F374973CD4BB3DC41E3081346798497FF6E36CB9352281DFE07D07FC530CA9AD8EF7AAD56EF5D41BE83D5E543807"),
            make_array ("3939FCC8B57B63612542DA31A834E5DCC36E2EE0F652AC72E02624FA2E5ADEECC7DD6BB3580224B4D6138706FC6E80597B528051230B00621CC2B22999EAA205"),
        },
        {
            " 16-bit string",
            make_vector ("41FB"),
            make_array ("BFF295861DAEDF33E70519B1E2BCB4C2E9FE3364D789BC3B17301C15"),
            make_array ("39F31B6E653DFCD9CAED2602FD87F61B6254F581312FB6EEEC4D7148FA2E72AA"),
            make_array ("1DD81609DCC290EFFD7AC0A95D4A20821580E56BD50DBD843920650BE7A80A1719577DA337CFDF86E51C764CAA2E10BD"),
            make_array ("AA092865A40694D91754DBC767B5202C546E226877147A95CB8B4C8F8709FE8CD6905256B089DA37896EA5CA19D2CD9AB94C7192FC39F7CD4D598975A3013C69"),
        },
        {
            "224-bit string",
            make_vector ("0F8B2D8FCFD9D68CFFC17CCFB117709B53D26462A3F346FB7C79B85E"),
            make_array ("1E693B0BCE2372550DAEF35B14F13AB43441ED6742DEE3E86FD1D8EF"),
            make_array ("6DE164A9626D5A4F54D854AC158994F35A8E362ECC753F55182790934A2E0D06"),
            make_array ("641A7AF13B889D1A0F1AA3E4E4FF8CC5903C47E1A52BDEA257D80E37E596564AB33EEAD06717CDB6B706CB6986293D4F"),
            make_array ("21132FC11F6040AD493D627027C752CE29816589DE7BE78562914B63D1A9219803DDBD9673AA749F37FF4D6E1B5AE2A12633BA8B0C9994E031EBF6C42E58A793"),
        },
        {
            "256-bit string",
            make_vector ("9F2FCC7C90DE090D6B87CD7E9718C1EA6CB21118FC2D5DE9F97E5DB6AC1E9C10"),
            make_array ("887921848AD98458F3DB3E0ECD5AD5DB1F0BF9F2D0CA08601074D597"),
            make_array ("2F1A5F7159E34EA19CDDC70EBF9B81F1A66DB40615D7EAD3CC1F1B954D82A3AF"),
            make_array ("BAAE7AAED4FBF42F9316C7E8F722EEB06A598B509F184B22FBD5A81C93D95FFF711F5DE90847B3248B6DF76CABCE07EE"),
            make_array ("B087C90421AEBF87911647DE9D465CBDA166B672EC47CCD4054A7135A1EF885E7903B52C3F2C3FE722B1C169297A91B82428956A02C631A2240F12162C7BC726"),
        },
        {
            "384-bit string",
            make_vector ("D8FABA1F5194C4DB5F176FABFFF856924EF627A37CD08CF55608BBA8F1E324D7C7F157298EABC4DCE7D89CE5162499F9"),
            make_array ("B7A51FBB084DEEB55136EFD7260E5B112E3C40D1A2D14B142DF930DF"),
            make_array ("34F8607EC10C092C1BA0B6565CE6197062C4E1A35A8E8C723E48A2D2416C3790"),
            make_array ("A127FEFCDD240F762CCE3F5F1551FC7E1CDEBC7950D1CD94C6888F490CB2285A10FD0EE797B168C5CA4761FA232AAF05"),
            make_array ("7EF3A2894C6ECBC4201B15348F90671515ACCBA3C8166621F864A9184BF08C3F5A895F6B599D3CB41F20A8A1DF25AE84F1A6D7C8DE74FB7CEF48F7E96FDE8D43"),
        },
        {
            "512-bit string",
            make_vector ("E926AE8B0AF6E53176DBFFCC2A6B88C6BD765F939D3D178A9BDE9EF3AA131C61E31C1E42CDFAF4B4DCDE579A37E150EFBEF5555B4C1CB40439D835A724E2FAE7"),
            make_array ("C154607F986F9BF902D831293C8386D36B201EABA6F6FB0B678B4B81"),
            make_array ("27A6441EE939B46E2C378D7AFEB0E891C47A28120E488EFF0AB71AF08788CEB3"),
            make_array ("423BA134D3BCB5E440AC83372C7EDDBA3AE3BDDF1222F505C19CDE246AD76A2B0D07239A54E1D0934C9B3D29D49E5FBD"),
            make_array ("EB5067BF762A291CF258AD69A816A0B089E0BD44F8E5B74CF60BCE64734E59853CCB8D091CD2E33F90AA063FB7942CF5965D459200144C1A0801ABD69A9A094A"),
        },
        {
            "520-bit string",
            make_vector ("16E8B3D8F988E9BB04DE9C96F2627811C973CE4A5296B4772CA3EEFEB80A652BDF21F50DF79F32DB23F9F73D393B2D57D9A0297F7A2F2E79CFDA39FA393DF1AC00"),
            make_array ("95E87AC90F541AB90CBCF7FD7E0E0C152CEF78D5EE1830E9ED8A1ED7"),
            make_array ("C4BB067383002DB44CA773918BB74104B604A583E12B06BE56C270F8B43512F2"),
            make_array ("662C4851D311A786DE4CDA7E9EA1EFF0BFA462761FF6CF804E591ED9A15B0DC93A2BB6A6CFFDC8D7D23A233A52C86EAD"),
            make_array ("B0E23D600BA4215F79D50047BBFED50DF7D6E769514D796AFD166DEECA88BD1CBE0AFC72A41E0317A223225B4F5882F723AFCBA3AF7C457EB525946DA6C53BB0"),
        },
        {
            "1,000,000 'a' string",
            std::vector<uint8_t> (1'000'000, 'a'),
            make_array ("d69335b93325192e516a912e6d19a15cb51c6ed5c15243e7a7fd653c"),
            make_array ("5c8875ae474a3634ba4fd55ec85bffd661f32aca75c6d699d0cdcb6c115891c1"),
            make_array ("eee9e24d78c1855337983451df97c8ad9eedf256c6334f8e948d252d5e0e76847aa0774ddb90a842190d2c558b4b8340"),
            make_array ("3c3a876da14034ab60627c077bb98f7e120a2a5370212dffb3385a18d4f38859ed311d0a9d5141ce9cc5c66ee689b266a8aa18ace8282a0e0db596c90b0a7b87"),
        }
    };

    for (const auto &t: TESTS) {
        #define TEST(WIDTH)                                                             \
        do {                                                                            \
            std::array<uint8_t, WIDTH/8> sha3_##WIDTH;                                  \
            std::fill (std::begin (sha3_##WIDTH), std::end (sha3_##WIDTH), 0);          \
                                                                                        \
            FIPS202_SHA3_##WIDTH (t.data.data (), t.data.size (), &sha3_##WIDTH[0]);    \
                                                                                        \
            tap.expect_eq (t.sha3_##WIDTH, sha3_##WIDTH, "sha3-%u %s", unsigned (WIDTH), t.msg);   \
        } while (0)

        TEST(224);
        TEST(256);
        TEST(384);
        TEST(512);
    }

    return tap.status ();
}
